package tiling.othercomponent;

import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.problems.objectives.evaluations.Evaluation;
import org.jamesframework.core.problems.sol.Solution;
import org.jamesframework.core.search.Search;
import org.jamesframework.core.search.listeners.SearchListener;
import tiling.gui.TilingFrame;
import tiling.solution.UrbanBlockSolution;

import java.util.concurrent.TimeUnit;

public class CreateSolutionListener implements SearchListener<Solution>{

  private TilingFrame frame;

  public CreateSolutionListener(TilingFrame frame){
    this.frame = frame;
  }

  @Override
  public void searchStarted(Search search){
    frame.setTitle("Create random solution");
    frame.setEvaluation("A start solution is creating");
  }

  @Override
  public void searchStopped(Search search){
    UrbanBlockSolution bestSolution = (UrbanBlockSolution) search.getBestSolution();
    frame.setGrid(bestSolution.getGrid());
    frame.setEvaluation("Start Solution");
    try{
      TimeUnit.MILLISECONDS.sleep(200);
    }catch(InterruptedException e){
      e.printStackTrace();
    }
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