package tiling.othercomponent;

import org.jamesframework.core.problems.constraints.validations.Validation;
import org.jamesframework.core.problems.sol.RandomSolutionGenerator;
import org.jamesframework.core.search.neigh.Move;
import org.jamesframework.core.search.neigh.Neighbourhood;
import tiling.constraint.RadiusConstraint;
import tiling.neighbourhood.ChangeColorMove;
import tiling.neighbourhood.ChangeColorNeighbourhood;
import tiling.solution.*;
import util.Tiling;
import util.TriFunction;

import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

public class RandomUrbanBlockGenerator implements RandomSolutionGenerator<UrbanBlockSolution, UrbanBlockData>{

  private final long maxTime;
  private final int width;
  private final int height;
  private final double sideLength;
  private final TriFunction<Integer, Integer, Double, Tiling> initTiling;
  private final HashMap<Integer, Integer> toLock;
  private final ArrayList<Function<UrbanBlockSolution, UrbanBlockInvariant>> invariants;
  private final Neighbourhood<UrbanBlockSolution> neighbourhood;


  public RandomUrbanBlockGenerator(long maxTime,
                                   TimeUnit timeUnit,
                                   int width, int height,
                                   double sideLength,
                                   TriFunction<Integer, Integer, Double, Tiling> initTiling,
                                   HashMap<Integer, Integer> toLock,
                                   ArrayList<Function<UrbanBlockSolution, UrbanBlockInvariant>> invariants,
                                   Neighbourhood<UrbanBlockSolution> neighbourhood){
    this.width = width;
    this.height = height;
    this.sideLength = sideLength;
    this.initTiling = initTiling;
    this.toLock = toLock;
    this.invariants = invariants;
    this.neighbourhood = neighbourhood;
    this.maxTime = timeUnit.toMillis(maxTime);

  }

  @Override
  public UrbanBlockSolution create(Random random, UrbanBlockData urbanBlockData){
    UrbanBlockSolution sol = new UrbanBlockSolution(initTiling.apply(width, height, sideLength));
    for(Function<UrbanBlockSolution, UrbanBlockInvariant> f : invariants){
      UrbanBlockInvariant inv = f.apply(sol);
      sol.addInvariant(inv);
    }

    for(int key : toLock.keySet()){
      sol.lockCell(key, toLock.get(key));
    }
    /*RadiusConstraint constraint = new RadiusConstraint(2);
    Validation validation = constraint.validate(sol, new UrbanBlockData());
    Instant start = Instant.now();
    Instant current = Instant.now();
    while(Duration.between(start, current).toMillis() < maxTime){
      Move< ? super UrbanBlockSolution> move = neighbourhood.getRandomMove(sol, random);
      validation = constraint.validate(move, sol, validation, new UrbanBlockData());

      if(validation.passed())
        move.apply(sol);

      current = Instant.now();
     }*/
    return sol;
  }
}
