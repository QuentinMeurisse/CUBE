package tiling.constraint;

import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;

import java.util.HashMap;

public class BuildingByBuildingValidation implements PenalizingValidation{

  private final double totalPenalty;
  private final HashMap<Integer, Double> penalties;

  public BuildingByBuildingValidation(double totalPenalty, HashMap<Integer, Double> penalties){
    this.totalPenalty = totalPenalty;
    this.penalties = penalties;
  }

  public HashMap<Integer, Double> getPenalties(){
    return new HashMap<>(penalties);
  }

  public double getTotalPenalty(){
    return totalPenalty;
  }

  @Override
  public double getPenalty(){
    if(penalties.size() == 0)
      return Double.MAX_VALUE;
    else
      return totalPenalty / penalties.size();
  }

  @Override
  public boolean passed(){
    return totalPenalty == 0.0;
  }
}
