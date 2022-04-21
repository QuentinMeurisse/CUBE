package tiling.othercomponent;

import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.problems.objectives.evaluations.Evaluation;
import org.jamesframework.core.problems.sol.Solution;
import org.jamesframework.core.search.Search;
import tiling.gui.TilingFrame;
import tiling.solution.GreenSpacesInvariant;
import tiling.solution.UrbanBlockSolution;

public class GreenSpacesListener extends TilingListener{

  public GreenSpacesListener(TilingFrame frame){
    super(frame);
  }

  @Override
  public void searchStopped(Search search){
    super.searchStopped(search);
    UrbanBlockSolution bestSolution = (UrbanBlockSolution) search.getBestSolution();
    GreenSpacesInvariant greenInv = (GreenSpacesInvariant) bestSolution.getInvariants().get(1);
    frame.setGreenSpaceArea("" + (greenInv.getGreenSpacesArea() / bestSolution.getGrid().getTotalArea()));
  }

  @Override
  public void newBestSolution(Search search,
                              Solution newBestSolution,
                              Evaluation newBestSolutionEvaluation,
                              Validation newBestSolutionValidation){
    super.newBestSolution(search, newBestSolution, newBestSolutionEvaluation, newBestSolutionValidation);
    UrbanBlockSolution bestSolution = (UrbanBlockSolution) newBestSolution;
    GreenSpacesInvariant greenInv = (GreenSpacesInvariant) bestSolution.getInvariants().get(1);
    frame.setGreenSpaceArea("" + (greenInv.getGreenSpacesArea() / bestSolution.getGrid().getTotalArea()));
  }
}
