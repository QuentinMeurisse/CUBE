package util.debug;

import org.jamesframework.core.search.neigh.Move;
import tiling.solution.UrbanBlockSolution;

import java.util.Iterator;
import java.util.ListIterator;

public class MoveDoubleLinkedList<M extends Move<UrbanBlockSolution>> implements Iterable<M>{


  private Node first, last;
  private long numNodes = 0;

  public UrbanBlockSolution sol;

  public MoveDoubleLinkedList(UrbanBlockSolution sol){
    first = null;
    last = null;
    this.sol = sol;
  }

  public void add(M m, boolean applied){
    if(numNodes == 0){
      first = new Node(m, null, null, applied);
      last = first;
      numNodes++;
    }
    else{
      Node n = new Node(m, last, null, applied);
      last.next = n;
      last = n;
      numNodes++;
    }
  }

  public long size(){
    return numNodes;
  }

  public M getLastMove(){
    return last.move;
  }

  @Override
  public Iterator<M> iterator(){
    return new DoubleLinkedListIterator(this);
  }

  private class Node{
    private M move;
    private Node prev, next;
    private boolean applied;

    public Node(M move, Node prev, Node next, boolean applied){
      this.move = move;
      this.prev = prev;
      this.next = next;
      this.applied = applied;
    }
  }

  public class DoubleLinkedListIterator implements ListIterator<M>{

    private Node prev, next;
    private int currentIndex = 0;

    public DoubleLinkedListIterator(MoveDoubleLinkedList<M> t){
      next = t.first;
      prev = null;

    }

    public boolean isNextApplied(){
      return next.applied;
    }

    public boolean isPreviousApplied(){
      return prev.applied;
    }

    @Override
    public boolean hasNext(){
      return next != null;
    }

    @Override
    public M next(){
      M currentMove = next.move;

      if(next.applied){
        System.out.println("applied move: " + currentMove);
        currentMove.apply(sol);
      }
      else{
        System.out.println("undo move: " + currentMove);
        currentMove.undo(sol);
      }

      prev = next;
      next = next.next;
      currentIndex++;

      return currentMove;
    }

    @Override
    public boolean hasPrevious(){
      return prev !=null;
    }

    @Override
    public M previous(){
      M prevMove = prev.move;

      if(prev.applied){
        System.out.println("applied move: " + prevMove);
        prevMove.undo(sol);
      }
      else{
        System.out.println("undp move: " + prevMove);
        prevMove.apply(sol);
      }

      next = prev;
      prev = prev.prev;
      currentIndex--;

      return prevMove;
    }

    @Override
    public int nextIndex(){
      return currentIndex;
    }

    @Override
    public int previousIndex(){
      return currentIndex -1;
    }

    @Override
    public void remove(){

    }

    @Override
    public void set(M m){

    }

    @Override
    public void add(M m){

    }
  }
}
