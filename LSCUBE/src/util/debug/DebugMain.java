package util.debug;

import tiling.gui.SquarePanel;
import tiling.neighbourhood.ChangeColorMove;
import tiling.solution.BuildingInvariant;
import tiling.solution.GreenSpacesInvariant;
import tiling.solution.UrbanBlockSolution;
import util.SquareTiling;

import java.util.ListIterator;

public class DebugMain{

  public static void main(String[] args){
    SquareTiling t = new SquareTiling(15, 15, 2);
    UrbanBlockSolution sol = new UrbanBlockSolution(t);
    BuildingInvariant buildInv = new BuildingInvariant(sol);
    sol.addInvariant(buildInv);
    GreenSpacesInvariant greenInv = new GreenSpacesInvariant(sol);
    sol.addInvariant(greenInv);
    JSONManager jm = new JSONManager();
    MoveDoubleLinkedList<ChangeColorMove> mddl = jm.read("/home/quentin/Bureau/pouet.json", sol);
    ListIterator<ChangeColorMove> iterator = (ListIterator<ChangeColorMove>) mddl.iterator();

    System.out.println(mddl.size());
    int x = 0;
    while(iterator.hasNext() && x < 43540){
      iterator.next();
      x++;
    }

    SquarePanel panel = new SquarePanel(t);
    DebugFrame frame = new DebugFrame(panel, iterator);
  }
}
