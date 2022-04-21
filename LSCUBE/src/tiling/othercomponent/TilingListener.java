package tiling.othercomponent;

import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.problems.objectives.evaluations.Evaluation;
import org.jamesframework.core.problems.sol.Solution;
import org.jamesframework.core.search.Search;
import org.jamesframework.core.search.listeners.SearchListener;
import tiling.gui.TilingFrame;
import tiling.solution.BuildingInvariant;
import tiling.solution.GreenSpacesInvariant;
import tiling.solution.UrbanBlockSolution;

import java.util.concurrent.TimeUnit;


public class TilingListener implements SearchListener<Solution>{

  protected TilingFrame frame;

  public TilingListener(TilingFrame frame){
    this.frame = frame;
  }

  @Override
  public void searchStopped(Search search){
    frame.finished();

    UrbanBlockSolution bestSolution = (UrbanBlockSolution) search.getBestSolution();
    BuildingInvariant buildInv = (BuildingInvariant) bestSolution.getInvariants().get(0);
    frame.setEvaluation(search.getBestSolutionEvaluation().toString());
    frame.setArea("" + buildInv.getBuildArea());
    frame.setPerimeter("" + buildInv.getBuildPerimeter());
    frame.setNumBuilding("" + buildInv.getNumBuilding());
    if (bestSolution.getInvariants().size() == 2){
      GreenSpacesInvariant ginv = (GreenSpacesInvariant) bestSolution.getInvariants().get(1);
      double green = ginv.getGreenSpacesArea() / bestSolution.getGrid().getTotalArea();
      frame.setGreenSpaceArea("" + green);
    }
    frame.setGrid(bestSolution.getGrid());
  }

  @Override
  public void newBestSolution(Search search,
                              Solution newBestSolution,
                              Evaluation newBestSolutionEvaluation,
                              Validation newBestSolutionValidation){

    UrbanBlockSolution bestSolution = (UrbanBlockSolution) newBestSolution;
    BuildingInvariant inv = (BuildingInvariant) bestSolution.getInvariants().get(0);
    frame.setEvaluation(search.getBestSolutionEvaluation().toString());
    frame.setArea("" + inv.getBuildArea());
    frame.setPerimeter("" + inv.getBuildPerimeter());
    frame.setNumBuilding("" + inv.getNumBuilding());
    if (bestSolution.getInvariants().size() == 2){
      GreenSpacesInvariant ginv = (GreenSpacesInvariant) bestSolution.getInvariants().get(1);
      double green = ginv.getGreenSpacesArea() / bestSolution.getGrid().getTotalArea();
      frame.setGreenSpaceArea("" + green);
    }
    frame.setGrid(bestSolution.getGrid());
    try{
      TimeUnit.MILLISECONDS.sleep(100);
    }catch(InterruptedException e){
      e.printStackTrace();
    }

  }

}
