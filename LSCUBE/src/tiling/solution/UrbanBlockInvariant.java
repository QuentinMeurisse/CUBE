package tiling.solution;

import util.Cell;
import util.Invariant;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Abstract class for the invariant on UrbanBlockSolution
 */
public abstract class UrbanBlockInvariant implements Invariant<UrbanBlockSolution>{

  protected UrbanBlockSolution observedSolution;
  protected HashMap<Integer, GroupOfCells> components = new HashMap<>();

  public abstract UrbanBlockInvariant copy(UrbanBlockSolution clonedSolution, HashMap<Cell, Cell> hm);

  /**
   * Use to distinct the different components according to their color
   * @param color
   * @return
   */
  protected abstract boolean colorCondition(int color);

  /**
   *
   * @param c
   * @return the component of the invariant that is expanded by adding c
   */
  public int getExpandedComponent(Cell c){
    for(Cell  neigh : c.getNeighbours().values()){
      int data = neigh.getData();
      if(data != 0 && colorCondition(neigh.getColor())) //We consider only neighbour with the right color
        return data;
    }
    return 0;
  }

  /**
   *
   * @param c the cell that can cause a fusion of some component
   * @param refColor the color of the components we want merge
   * @return the components that can be merged by coloring c
   */
  public ArrayList<GroupOfCells> mergedMaterial(Cell c, int refColor){
    ArrayList<GroupOfCells> toReturn = new ArrayList<>();
    for(Cell d : c.getNeighbours().values()){
      //We check if neighbour of c have the same color
      if(d.getColor() != 0
              && d.getColor() == refColor
              && !(toReturn.contains(components.get(d.getData()))))
        toReturn.add(components.get(d.getData()));
    }
    return toReturn;
  }
}
