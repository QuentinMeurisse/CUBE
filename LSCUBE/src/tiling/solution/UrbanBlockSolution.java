package tiling.solution;

import org.jamesframework.core.problems.sol.Solution;
import org.jamesframework.core.search.neigh.Move;
import tiling.neighbourhood.ChangeColorMove;
import util.Cell;
import util.Invariant;
import util.Tiling;

import java.util.ArrayList;
import java.util.HashMap;

public class UrbanBlockSolution extends Solution{


  private Tiling grid;

  /**
   * Invariant 0 is supposed to be a BuildingInvariant.
   * Invariant 1 is supposed to ba GreenSpacesInvariant.
   */
  private ArrayList<UrbanBlockInvariant> invariants = new ArrayList<>();

  public UrbanBlockSolution(Tiling grid){
    this.grid = grid;
  }

  public Tiling getGrid(){
    return grid;
  }

  public GroupOfCells getComponents(int id, int color){
    GroupOfCells toReturn = null;
    if(color > 0){
      BuildingInvariant inv = (BuildingInvariant) invariants.get(0);
      toReturn = inv.getBuildingOfId(id);
    }
    else if(color == -1){
      GreenSpacesInvariant invariant = (GreenSpacesInvariant) invariants.get(1);
      toReturn = invariant.getGreenSpaceOfId(id);
    }
    return toReturn;
  }

  public UrbanBlockInvariant getCorrespondingInvariant(int color){
    UrbanBlockInvariant toReturn = null;
    if(color > 0)
      toReturn = invariants.get(0);
    else if(color == -1)
      toReturn = invariants.get(1);

    return toReturn;
  }

  public void lockCell(int k, int color){
    Cell c = grid.getCell(k);
    ChangeColorMove move = new ChangeColorMove(k, color);
    move.apply(this);
    c.lock();
  }


  public void addInvariant(UrbanBlockInvariant inv){
    invariants.add(inv);
  }

  public void upgradeAll(Move<UrbanBlockSolution> move){
    for(Invariant<UrbanBlockSolution> inv : invariants){
      inv.upgrade(move);
    }
  }

  public void downgradeAll(Move<UrbanBlockSolution> move){
    for(Invariant<UrbanBlockSolution> inv : invariants)
      inv.downgrade(move);
  }

  public void modifyColor(int k, int color){
    Cell c = grid.getCell(k);
    c.setColor(color);
  }

  public ArrayList<UrbanBlockInvariant> getInvariants(){
    return invariants;
  }

  @Override
  public Solution copy(){
    HashMap<Cell, Cell> hm = grid.mapForClone();
    UrbanBlockSolution clonedSolution = new UrbanBlockSolution(grid.clone(hm));
    for(UrbanBlockInvariant inv : invariants){
      UrbanBlockInvariant clonedInv = inv.copy(clonedSolution, hm);
      clonedSolution.addInvariant(clonedInv);
    }
    return clonedSolution;
  }

  @Override
  public boolean equals(Object o){
    if(!(o instanceof UrbanBlockSolution))
      return false;
    UrbanBlockSolution other = (UrbanBlockSolution) o;
    return this.grid.equals(other.grid);
  }

  @Override
  public int hashCode(){
    return grid.hashCode();
  }

  @Override
  public String toString(){
    return grid.toString();
  }

}
