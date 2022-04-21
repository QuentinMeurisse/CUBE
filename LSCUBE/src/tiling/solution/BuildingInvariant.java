package tiling.solution;

import org.jamesframework.core.search.neigh.Move;
import tiling.neighbourhood.ChangeColorMove;
import util.Cell;
import util.Utils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;

public class BuildingInvariant extends UrbanBlockInvariant{

  private int numBuilding = 0;
  private double buildArea = 0;
  private double buildPerimeter = 0;

  private int ids = 0;
  private Stack<Integer> reusableIds = new Stack<>();


  public BuildingInvariant(UrbanBlockSolution observedSolution){
    this.observedSolution = observedSolution;
  }

  public int getNumBuilding(){
    return numBuilding;
  }

  public double getBuildArea(){
    return buildArea;
  }

  public double getBuildPerimeter(){
    return buildPerimeter;
  }

  public GroupOfCells getBuildingOfId(int id){
    return components.get(id);
  }

  public HashMap<Integer, GroupOfCells> getBuildings(){
    return components;
  }


  @Override
  public void upgrade(Move<UrbanBlockSolution> move){
    if(move instanceof ChangeColorMove){
      ChangeColorMove ccm = (ChangeColorMove) move;
      changeColorUpgrade(ccm);
    }
  }

  @Override
  public void downgrade(Move<UrbanBlockSolution> move){
    if(move instanceof ChangeColorMove){
      ChangeColorMove ccm = (ChangeColorMove) move;
      changeColorDowngrade(ccm);
    }
  }

  @Override
  public BuildingInvariant copy(UrbanBlockSolution clonedSolution, HashMap<Cell, Cell> hm){
    BuildingInvariant toReturn = new BuildingInvariant(clonedSolution);

    toReturn.numBuilding = this.numBuilding;
    toReturn.buildPerimeter = this.buildPerimeter;
    toReturn.buildArea = this.buildArea;
    toReturn.ids = this.ids;
    toReturn.reusableIds = Utils.copyStack(reusableIds);
    for(int key : components.keySet()){
      GroupOfCells b = this.components.get(key).copy(hm);
      toReturn.components.put(key, b);
    }
    return toReturn;

  }

  @Override
  protected boolean colorCondition(int color){
    return color > 0;
  }

  private void changeColorUpgrade(ChangeColorMove move){
    int k = move.getK();
    int newColor = move.getNewColor();
    int oldData = move.getOldData();
    Cell c = observedSolution.getGrid().getCell(k);
    if(oldData <= 0 && newColor > 0){//If we are working on a empty cell or a not build cell
      int x = getExpandedComponent(c);
      if(x == 0){ //If we create a new building
        int id = reusableIds.isEmpty() ? ++ids : reusableIds.pop(); //We get an id
        c.setColor(newColor);
        c.setData(id);
        GroupOfCells b = new GroupOfCells(id, c);
        components.put(id, b); //We add the new component in the HashMap
        //We update the properties
        buildArea += c.getArea();
        buildPerimeter += c.getPerimeter();
        numBuilding++;
      }else{
        GroupOfCells b = components.get(x);
        ArrayList<GroupOfCells> base = mergedMaterial(c, b.getColor());
        if(base.size() > 1){//We are merging buildings
          for(GroupOfCells d : base)
            components.remove(d.getId()); //We remove the merged components

          MergedGroupOfCells merged = new MergedGroupOfCells(++ids, base); //We create a bigger building
          components.put(ids, merged);
          b = merged;
          numBuilding -= (base.size() - 1);
        }
        double oldArea = b.getArea();
        double oldPerimeter = b.getPerimeter();
        b.addCell(c); //We add the cell to our component and update the properties
        buildArea += (b.getArea() - oldArea);
        buildPerimeter += (b.getPerimeter() - oldPerimeter);
      }
    }else if(oldData > 0){ //If we are working on a build cell
      GroupOfCells b = components.get(oldData);
      if(newColor < 0){ //If we change a build cell in a green cell for example
        double oldArea = b.getArea();
        double oldPerimeter = b.getPerimeter();
        b.removeCell(c);
        buildArea += (b.getArea() - oldArea);
        buildPerimeter += (b.getPerimeter() - oldPerimeter);
        if(b.getArea() <= 1e-12){ //manage approximation error
          numBuilding--;
          components.remove(b.getId());
          reusableIds.push(b.getId());
        }
      }
      else{//We just change the color of a building
        b.changeColor(newColor);
        if(newColor == 0){//We remove a building
          buildPerimeter -= b.getPerimeter();
          buildArea -= b.getArea();
          reusableIds.push(b.getId());
          components.remove(b.getId());
          numBuilding--;
        }
      }
    }
  }

  private void changeColorDowngrade(ChangeColorMove move){
    int k = move.getK();
    int oldColor = move.getOldColor();
    int newColor = move.getNewColor();
    Cell c = observedSolution.getGrid().getCell(k);
    GroupOfCells lastTouched = move.getCurrentGroupOfCells();
    if(newColor > 0 || oldColor > 0){ //We undo a move only if it concern a build cell
      if(move.isRecolored()){ //If the move was just recolor a building
        lastTouched.changeColor(oldColor);
        if(newColor == 0){ //If the move was removing a building
          buildArea += lastTouched.getArea();
          buildPerimeter += lastTouched.getPerimeter();
          numBuilding++;
          int popId = reusableIds.pop();
          components.put(popId, lastTouched);
        }
      }else if(move.isExpanded()){
        if(oldColor != 0)//If we expand the building on a no-build cell
          changeColorUpgrade(move.reverse());
        else{//To avoid a total deletion of the building
          double expandedArea = lastTouched.getArea();
          double expandedPerimeter = lastTouched.getPerimeter();
          lastTouched.removeCell(c);
          buildArea += (lastTouched.getArea() - expandedArea);
          buildPerimeter += (lastTouched.getPerimeter() - expandedPerimeter);
        }
      }else if(move.isMerged()){
        if(newColor > 0){ //If the move was merging some buildings
          MergedGroupOfCells mergedBuilding = (MergedGroupOfCells) lastTouched;
          buildArea -= mergedBuilding.getArea();
          buildPerimeter -= mergedBuilding.getPerimeter();
          mergedBuilding.removeCell(c);
          components.remove(mergedBuilding.getId());
          numBuilding--;
          ArrayList<GroupOfCells> base = mergedBuilding.split();
          for(GroupOfCells b : base){
            b.correctCellsData();
            buildArea += b.getArea();
            buildPerimeter += b.getPerimeter();
            components.put(b.getId(), b);
            numBuilding++;
          }
          reusableIds.push(mergedBuilding.getId());
        }
        else//The move was merging other components from a build cell
          changeColorUpgrade(move.reverse());
      }else//In other case, the move can be undo by just changing the color of the cell k
        changeColorUpgrade(move.reverse());
    }
  }
}
