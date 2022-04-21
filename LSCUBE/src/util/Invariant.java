package util;

import org.jamesframework.core.problems.sol.Solution;
import org.jamesframework.core.search.neigh.Move;

/**
 * An invariant observe a solution and keeps some property on it.
 * @param <T>
 */
public interface Invariant<T extends Solution>{

  /**
   *
   * @param move Update the property according to move
   */
  void upgrade(Move<T> move);

  /**
   *
   * @param move undo the last update according to the move
   */
  void downgrade(Move<T> move);
}
