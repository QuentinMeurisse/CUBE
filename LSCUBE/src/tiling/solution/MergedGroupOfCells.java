package tiling.solution;

import util.Cell;

import java.util.ArrayList;
import java.util.HashMap;

public class MergedGroupOfCells extends GroupOfCells{


  private ArrayList<GroupOfCells> base;


  public MergedGroupOfCells(int id, ArrayList<GroupOfCells> base){
    super(id);
    this.base = base;
    this.color = base.get(0).color;
    for(GroupOfCells b : base){
      area += b.area;
      perimeter += b.perimeter;
      for(Cell c : b.inner){
        c.setData(id);
        this.inner.add(c);
      }
      for(Cell c : b.border){
        c.setData(id);
        this.border.add(c);
      }
    }
  }

  public ArrayList<GroupOfCells> split(){
    return base;
  }

  public GroupOfCells copy(HashMap<Cell, Cell> hm){
    GroupOfCells clonedGroupOfCells = super.copy(hm);
    ArrayList<GroupOfCells> clonedBase = new ArrayList<>();
    for(GroupOfCells b : base){
      clonedBase.add(b.copy(hm));
    }
    MergedGroupOfCells cloned = new MergedGroupOfCells(this.id, clonedBase);
    cloned.border = clonedGroupOfCells.border;
    cloned.inner = clonedGroupOfCells.inner;
    cloned.area = clonedGroupOfCells.area;
    cloned.perimeter = clonedGroupOfCells.perimeter;
    cloned.color = clonedGroupOfCells.color;
    return cloned;
  }
}
