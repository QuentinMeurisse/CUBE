package tiling.constraint;

import org.jamesframework.core.problems.constraints.PenalizingConstraint;
import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;
import tiling.solution.BuildingInvariant;
import tiling.solution.GroupOfCells;
import tiling.solution.UrbanBlockData;
import tiling.solution.UrbanBlockSolution;
import util.Tiling;

import java.util.HashMap;

public class BuildingsPerimeterConstraint implements PenalizingConstraint<UrbanBlockSolution, UrbanBlockData>{

  private final double minPerimeter;
  private final double maxPerimeter;
  private final boolean normalized;

  public BuildingsPerimeterConstraint(double minPerimeter, double maxPerimeter, boolean normalized){
    this.minPerimeter = minPerimeter;
    this.maxPerimeter = maxPerimeter;
    this.normalized = normalized;
  }

  @Override
  public PenalizingValidation validate(UrbanBlockSolution solution, UrbanBlockData urbanBlockData){
    double totalPenalty = 0;
    BuildingInvariant inv = (BuildingInvariant) solution.getInvariants().get(0);
    HashMap<Integer, GroupOfCells> buildings = inv.getBuildings();
    HashMap<Integer, Double> penalties = new HashMap<>();
    for(int key : buildings.keySet()){
      if(key >= 0){
        GroupOfCells b = buildings.get(key);
        double penalty;
        if(normalized){
          Tiling t = solution.getGrid();
          penalty = computeNormalizedPenalty(b, t.maxPerimeter());
        }
        else
          penalty = computePenalty(b);

        penalties.put(key, penalty);
        totalPenalty += penalty;
      }
    }
    return new BuildingByBuildingValidation(totalPenalty, penalties);
  }

  private double computeNormalizedPenalty(GroupOfCells b, double maxPossiblePerimeter){
    double penalty = 0;
    if(minPerimeter - b.getPerimeter() > 1e-10)
      penalty += (minPerimeter - b.getPerimeter()) / minPerimeter;
    if(b.getPerimeter() - maxPerimeter > 1e-10)
      penalty += (b.getPerimeter() - maxPerimeter) / (maxPossiblePerimeter - maxPerimeter);
    return penalty;
  }

  private double computePenalty(GroupOfCells b){
    double penalty = 0;
    if(minPerimeter - b.getPerimeter() > 1e-10)
      penalty += minPerimeter - b.getPerimeter();
    if(b.getPerimeter() - maxPerimeter > 1e-10)
      penalty += b.getPerimeter() - maxPerimeter;
    return penalty;
  }
}
