package tiling.othercomponent;

import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.problems.objectives.evaluations.Evaluation;
import org.jamesframework.core.search.Search;
import org.jamesframework.core.search.listeners.SearchListener;
import tiling.solution.UrbanBlockSolution;

public class ProgressSearchListener implements SearchListener<UrbanBlockSolution>{

  @Override
  public void searchStarted(Search search){
    System.out.println(">>> Search started");
  }

  @Override
  public void newBestSolution(Search search,
                              UrbanBlockSolution newBestSolution,
                              Evaluation newBestSolutionEvaluation,
                              Validation newBestSolutionValidation){

    System.out.println("New best solution evaluation : " + newBestSolutionEvaluation);
  }

  @Override
  public void searchStopped(Search search){
    System.out.println("Best solution evaluation : " + search.getBestSolutionEvaluation());

  }
}
