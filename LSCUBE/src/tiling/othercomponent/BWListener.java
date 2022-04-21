package tiling.othercomponent;

import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.problems.objectives.evaluations.Evaluation;
import org.jamesframework.core.problems.sol.Solution;
import org.jamesframework.core.search.Search;
import org.jamesframework.core.search.listeners.SearchListener;
import tiling.gui.BWFrame;
import tiling.solution.UrbanBlockSolution;

public class BWListener implements SearchListener<Solution>{

  private BWFrame frame;

  public BWListener(BWFrame frame){
    this.frame = frame;
  }

  @Override
  public void searchStopped(Search search){
    UrbanBlockSolution bestSolution = (UrbanBlockSolution) search.getBestSolution();
    frame.setGrid(bestSolution.getGrid());
  }

  @Override
  public void newBestSolution(Search search,
                              Solution newBestSolution,
                              Evaluation newBestSolutionEvaluation,
                              Validation newBestSolutionValidation){

    UrbanBlockSolution bestSolution = (UrbanBlockSolution) newBestSolution;
    frame.setGrid(bestSolution.getGrid());

  }
}
