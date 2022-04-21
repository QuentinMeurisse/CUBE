package tiling.neighbourhood;

import org.jamesframework.core.search.neigh.Move;
import tiling.solution.GroupOfCells;
import tiling.solution.UrbanBlockInvariant;
import tiling.solution.UrbanBlockSolution;
import util.Cell;

import java.util.ArrayList;

public class ChangeColorMove implements Move<UrbanBlockSolution>{

  private final int k;
  private final int newColor;

  private int oldColor;
  private int oldData;
  private GroupOfCells currentGroupOfCells;
  private int newData;

  private boolean merged = false;
  private boolean expanded = false;
  private boolean recolored = false;

  public ChangeColorMove(int k, int newColor){
    this.k = k;
    this.newColor = newColor;
  }

  public boolean isMerged(){
    return merged;
  }

  public boolean isExpanded(){
    return expanded;
  }

  public boolean isRecolored(){
    return recolored;
  }

  public GroupOfCells getCurrentGroupOfCells(){
    return currentGroupOfCells;
  }

  public int getK(){
    return k;
  }

  public int getNewColor(){
    return newColor;
  }

  public int getOldColor(){
    return oldColor;
  }

  public int getOldData(){
    return oldData;
  }

  public int getNewData(){
    return newData;
  }

  public ChangeColorMove reverse(){
    ChangeColorMove toReturn = new ChangeColorMove(k, oldColor);
    toReturn.oldColor = this.newColor;
    toReturn.oldData = this.newData;
    toReturn.newData = this.oldData;
    toReturn.currentGroupOfCells = this.currentGroupOfCells;
    return toReturn;
  }

  @Override
  public void apply(UrbanBlockSolution urbanBlockSolution){
    Cell c = urbanBlockSolution.getGrid().getCell(k);
    oldColor = c.getColor();
    oldData = c.getData();
    if(c.getColor() == 0 || c.getColor() * newColor < 0){ // We are working on a cell of another component
      UrbanBlockInvariant inv = urbanBlockSolution.getCorrespondingInvariant(newColor);
      int x = inv.getExpandedComponent(c);
      oldColor = c.getColor();
      if(x != 0){
        GroupOfCells b = urbanBlockSolution.getComponents(x, newColor);
        ArrayList<GroupOfCells> base = inv.mergedMaterial(c, b.getColor());
        if(base.size() > 1){
          merged = true;
        }
        else{
          expanded = true;
          currentGroupOfCells = b;
        }
      }
    }
    else{ // We are working on a colored cell. So it's just a color change
      recolored = true;
      oldColor = c.getColor();
      currentGroupOfCells = urbanBlockSolution.getComponents(c.getData(), oldColor);
    }

    //In all case, wa are changing the color of the cell number k

    urbanBlockSolution.modifyColor(k, newColor);
    urbanBlockSolution.upgradeAll(this);
    newData = c.getData();
    if(merged) // If we are merging buildings, we save the merged building for later
      currentGroupOfCells = urbanBlockSolution.getComponents(c.getData(), c.getColor());
  }

  @Override
  public void undo(UrbanBlockSolution urbanBlockSolution){
    urbanBlockSolution.modifyColor(k, oldColor);
    urbanBlockSolution.downgradeAll(this);
  }

  @Override
  public String toString(){
    return "ChangeColorMove{" +
            "k=" + k +
            ", newColor=" + newColor +
            ", oldColor=" + oldColor +
            ", oldData=" + oldData +
            ", merged=" + merged +
            ", expanded=" + expanded +
            ", recolored=" + recolored +
            '}';
  }
}
