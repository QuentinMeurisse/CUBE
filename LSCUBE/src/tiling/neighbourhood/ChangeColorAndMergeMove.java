package tiling.neighbourhood;

import org.jamesframework.core.search.neigh.Move;
import tiling.solution.UrbanBlockSolution;
import util.Cell;
import util.SetOperations;
import util.Tiling;

import java.util.*;

public class ChangeColorAndMergeMove implements Move<UrbanBlockSolution>{

  private final int k;
  private final int newColor;
  private final int radius;
  private Stack<ChangeColorMove> stack = new Stack<>();

  public ChangeColorAndMergeMove(int k, int newColor, int radius){
    this.k = k;
    this.newColor = newColor;
    this.radius = radius;
  }

  public int getK(){
    return k;
  }

  public int getNewColor(){
    return newColor;
  }

  public Stack<ChangeColorMove> getStack(){
    return stack;
  }

  @Override
  public void apply(UrbanBlockSolution urbanBlockSolution){
    ChangeColorMove firstMove = new ChangeColorMove(k, newColor);
    firstMove.apply(urbanBlockSolution);
    stack.push(firstMove);
    Tiling t = urbanBlockSolution.getGrid();
    Cell c = t.getCell(k);
    HashSet<Cell> cellsToPromptMerge = cellsToPromptTheMerge(c);
    for(Cell d : cellsToPromptMerge){
      int index = t.indexOf(d);
      ChangeColorMove nextMove = new ChangeColorMove(index, newColor);
      nextMove.apply(urbanBlockSolution);
      stack.push(nextMove);
    }

  }

  @Override
  public void undo(UrbanBlockSolution urbanBlockSolution){
    while(!stack.isEmpty()){
      ChangeColorMove move = stack.pop();
      move.undo(urbanBlockSolution);
    }
  }


  private HashSet<Cell> cellsToPromptTheMerge(Cell c){
    HashSet<Cell> toReturn = new HashSet<>();
    ArrayList<HashSet<Cell>> spheres = new ArrayList<>();
    for(int i = 1; i < radius; i++)
      spheres.add(c.sphere(i));

    Cell current = possiblyMerged(c);
    if(current != null){
      for(int i = spheres.size() - 1; i >= 0; i--){
        HashSet<Cell> neigh = new HashSet<>(current.getNeighbours().values());
        Set<Cell> intersection = SetOperations.intersection(spheres.get(i), neigh);
        Iterator<Cell> it = intersection.iterator();
        do{
          current = it.next();
        }while(it.hasNext() && current.isLocked());

        if(!current.isLocked())
          toReturn.add(current);
      }
    }
    return toReturn;
  }

  private Cell possiblyMerged(Cell c){
    HashSet<Cell> sphere = c.sphere(radius);
    for(Cell d : sphere){
      if(d.getColor() == c.getColor() && d.getData() != c.getData())
        return d;
    }
    return null;
  }

}
