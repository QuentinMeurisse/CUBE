package tiling.solution;

import org.jamesframework.core.search.neigh.Move;
import tiling.neighbourhood.ChangeColorMove;
import util.Cell;
import util.Utils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Stack;

public class GreenSpacesInvariant extends UrbanBlockInvariant{

  private double greenSpacesArea = 0;

  private int ids = 0;
  private Stack<Integer> reusableIds = new Stack<>();


  public GreenSpacesInvariant(UrbanBlockSolution observedSolution){
    this.observedSolution = observedSolution;
  }

  public int numGreenSpaces(){
    return components.size();
  }

  public double getGreenSpacesArea(){
    return greenSpacesArea;
  }

  public HashMap<Integer, GroupOfCells> getGreenSpaces(){
    return components;
  }

  public GroupOfCells getGreenSpaceOfId(int id){
    return components.get(id);
  }

  @Override
  public GreenSpacesInvariant copy(UrbanBlockSolution clonedSolution, HashMap<Cell, Cell> hm){
    GreenSpacesInvariant toReturn = new GreenSpacesInvariant(clonedSolution);
    toReturn.greenSpacesArea = this.greenSpacesArea;
    toReturn.ids = this.ids;
    toReturn.reusableIds = Utils.copyStack(this.reusableIds);
    for(int key : components.keySet()){
      GroupOfCells b = this.components.get(key).copy(hm);
      toReturn.components.put(key, b);
    }
    return toReturn;
  }

  @Override
  protected boolean colorCondition(int color){
    return color == -1;
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
    if(move instanceof ChangeColorMove)
      changeColorDowngrade((ChangeColorMove) move);

  }

  private void changeColorUpgrade(ChangeColorMove move){
    int k = move.getK();
    int newColor = move.getNewColor();
    int oldData = move.getOldData();
    int oldColor = move.getOldColor();
    Cell c  = observedSolution.getGrid().getCell(k);
    if(oldData >= 0 && newColor == -1){ //If we are changing a empty or a build cell into a green cell
      int x = getExpandedComponent(c);

      if(x >= 0){//We are creating a new green space
        int id = reusableIds.isEmpty() ? --ids : reusableIds.pop();
        c.setColor(newColor);
        c.setData(id);
        GroupOfCells b = new GroupOfCells(id, c);
        components.put(id, b);
        greenSpacesArea += c.getArea();
      }
      else{//We are expanding a green space
        GroupOfCells green = components.get(x);
        ArrayList<GroupOfCells> base = mergedMaterial(c, -1);
        if(base.size() > 1){ //We are merging some green spaces
          for(GroupOfCells h : base)
            components.remove(h.getId());

          MergedGroupOfCells merged = new MergedGroupOfCells(--ids, base);
          components.put(ids, merged);
          green = merged;
        }
        double oldArea = green.getArea();
        green.addCell(c);
        greenSpacesArea += (green.getArea() - oldArea);
      }
    }
    else if(oldColor == -1){ //We are recoloring a green cell
      GroupOfCells green = components.get(oldData);
      if(newColor == 0){ //We remove a green space
        green.changeColor(0);
        greenSpacesArea -= green.getArea();
        reusableIds.push(green.getId());
        components.remove(green.getId());
      }
      else{ //newColor != 0
        double oldArea = green.getArea();
        int currentData = c.getData();
        green.removeCell(c);
        c.setColor(newColor); //When we remove a cell, we recolor it in white. So we need to correct the color
        c.setData(currentData);// and the data
        greenSpacesArea += (green.getArea() - oldArea);
        if(green.getArea() <= 1e-12){
          components.remove(green.getId());
          reusableIds.push(green.getId());
        }
      }
    }
  }

  private void changeColorDowngrade(ChangeColorMove move){
    int k = move.getK();
    int oldColor = move.getOldColor();
    int newColor = move.getNewColor();
    if(newColor == -1 || oldColor == -1){ //If the move concern a green cell
      Cell c = observedSolution.getGrid().getCell(k);
      GroupOfCells lastTouched = move.getCurrentGroupOfCells();
      if(move.isExpanded()){
        if(oldColor != 0)
          changeColorUpgrade(move.reverse());
        else{
          double expandedArea = lastTouched.getArea();
          lastTouched.removeCell(c);
          greenSpacesArea += (lastTouched.getArea() - expandedArea);
        }
      }else if(move.isMerged()){
        if(newColor == -1){ //We merged two green spaces
          MergedGroupOfCells mergedGreenSpace = (MergedGroupOfCells) lastTouched;
          greenSpacesArea -= mergedGreenSpace.getArea();
          int currentColor = oldColor == 0 ? 0 : c.getColor();
          int currentData = oldColor == 0 ? 0 : c.getData();
          mergedGreenSpace.removeCell(c);
          components.remove(mergedGreenSpace.getId());
          c.setData(currentData);
          c.setColor(currentColor);
          ArrayList<GroupOfCells> base = mergedGreenSpace.split();
          for(GroupOfCells g : base){
            g.correctCellsData();
            greenSpacesArea += g.getArea();
            components.put(g.getId(), g);
          }
          reusableIds.push(mergedGreenSpace.getId());
        }
        else{ //We merged another component on a green spaces
          changeColorUpgrade(move.reverse());
        }
      }
      else if(move.isRecolored()){
        lastTouched.changeColor(oldColor);
        if(newColor == 0){
          greenSpacesArea += lastTouched.getArea();
          int popId = reusableIds.pop();
          components.put(popId, lastTouched);
        }
      }
      else{// The move was creating a new component
        changeColorUpgrade(move.reverse());
      }
    }
  }

}
