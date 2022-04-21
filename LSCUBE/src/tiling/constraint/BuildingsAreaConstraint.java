package tiling.constraint;

import org.jamesframework.core.problems.constraints.PenalizingConstraint;
import org.jamesframework.core.problems.constraints.validations.PenalizingValidation;
import tiling.solution.BuildingInvariant;
import tiling.solution.GroupOfCells;
import tiling.solution.UrbanBlockData;
import tiling.solution.UrbanBlockSolution;
import util.Tiling;

import java.util.HashMap;

public class BuildingsAreaConstraint implements PenalizingConstraint<UrbanBlockSolution, UrbanBlockData>{

  private final double minArea;
  private final double maxArea;
  private final boolean normalized;

  public BuildingsAreaConstraint(double minArea, double maxArea, boolean normalized){
    this.minArea = minArea;
    this.maxArea = maxArea;
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
          penalty = computeNormalizedPenalty(b, t.getTotalArea());
        }
        else
          penalty = computePenalty(b);

        penalties.put(key, penalty);
        totalPenalty += penalty;
      }
    }
    return new BuildingByBuildingValidation(totalPenalty, penalties);
  }

  private double computeNormalizedPenalty(GroupOfCells b, double maxPossibleArea){
    double penalty = 0;
    if(minArea - b.getArea() > 1e-10)
      penalty = (minArea - b.getArea()) / minArea;
    if(b.getArea() - maxArea > 1e-10)
      penalty = (b.getArea() - maxArea) / (maxPossibleArea - maxArea);
    return penalty;
  }

  private double computePenalty(GroupOfCells b){
    double penalty = 0;
    if(minArea - b.getArea() > 1e-10)
      penalty += minArea - b.getArea();
    if(b.getArea() - maxArea > 1e-10)
      penalty += b.getArea() - maxArea;
    return penalty;
  }

}
