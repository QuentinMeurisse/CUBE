package util;

import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.stream.IntStream;

import static java.lang.Math.*;

/**
 * Class which model a cell of tiling. A cell is supposed to be a regular polygon
 */
public class Cell{

  private final int numNeighbours;
  private int numSides;
  /*Two cells are neighbours if they have at least one intersection point.
    Neighbours are numbered from 0 in clockwise direction */
  private HashMap<Integer, Cell> neighbours;
  private int data;
  private int[] sidesNeighbours;

  private double sideLength;
  private double perimeter;
  private double area;

  private boolean locked = false;
  private int color = 0;

  /**
   *
   * @param numSides Sides number of the polygon
   * @param numNeighbours Number of neighbours.
   *                      NumNeighbours is not always equal to numSides. For example, a square have 4
   *                      sides and 8 neighbours
   * @param sidesNeighbours Key of the neighbours that have a side in common with the current cell
   * @param data Number contained in the cell
   * @param sideLength Side length of the polygon
   */
  public Cell(int numSides, int numNeighbours, int[] sidesNeighbours, int data, double sideLength){
    this.numNeighbours = numNeighbours;
    neighbours = new HashMap<>(numNeighbours);
    this.data = data;
    this.numSides = numSides;
    this.sideLength = sideLength;
    perimeter = numSides * sideLength;
    area = (sideLength * sideLength * numSides) /(4 * tan(PI / numSides));
    this.sidesNeighbours = sidesNeighbours;
  }

  public Cell(int numSides, int numNeighbours, double sideLength){
    this.numSides = numSides;
    this.numNeighbours = numNeighbours;
    this.sideLength = sideLength;
    neighbours = new HashMap<>(numNeighbours);
    data = 0;
    perimeter = numSides * sideLength;
    area = (sideLength * sideLength * numSides) / (4 * tan(PI / numSides));
    sidesNeighbours = IntStream.range(0, numSides).toArray();
  }

  public Cell(int numSides, int numNeighbours, int[] sidesNeighbours){
    this.numNeighbours = numNeighbours;
    this.numSides = numSides;
    this.sidesNeighbours = sidesNeighbours;
    neighbours = new HashMap<>(numNeighbours);
    data = 0;
    this.sideLength = 2;
    perimeter = numSides * sideLength;
    area = (sideLength * sideLength * numSides) / (4 * tan(PI / numSides));
  }

  public Cell(int numSides, int numNeighbours){
    this.numNeighbours = numNeighbours;
    neighbours = new HashMap<>(numNeighbours);
    this.data = 0;
    this.numSides = numSides;
    this.sideLength = 2;
    perimeter = numSides * sideLength;
    area = (sideLength * sideLength * numSides) / (4 * tan(PI / numSides));
    sidesNeighbours = IntStream.range(0, numSides).toArray();
  }

  public void addNeighbour(int key, Cell c){
    if(neighbours.size() == numNeighbours)
      throw new TooMuchNeighbourException("Cannot add an additional neighbour. " +
              "Maximum number of neighbours is " + numNeighbours);
    neighbours.put(key, c);
  }

  public int getNumNeighbours(){
    return numNeighbours;
  }

  public HashMap<Integer, Cell> getNeighbours(){
    return neighbours;
  }

  public int getData(){
    return data;
  }

  public void setData(int data){
    this.data = data;
  }

  public int getNumSides(){
    return numSides;
  }

  public double getSideLength(){
    return sideLength;
  }

  public double getPerimeter(){
    return perimeter;
  }

  public double getArea(){
    return area;
  }

  public int[] getSidesNeighbours(){
    return sidesNeighbours;
  }

  public boolean isEmptyCell(){
    return data == 0;
  }

  public void lock(){
    locked = true;
  }

  public void unlock(){
    locked = false;
  }

  public boolean isLocked(){
    return locked;
  }

  public boolean isANeighbourLocked(){
    for(Cell c : neighbours.values()){
      if(c.locked)
        return true;
    }
    return false;
  }

  public boolean isALockCellClose(){
    return this.locked || isANeighbourLocked();
  }

  public int getColor(){
    return color;
  }

  public void setColor(int color){
    this.color = color;
  }

  public Cell getNeighbour (int key){
    return neighbours.get(key);
  }

  public boolean areAllNeighboursFilled(){
    for(int key : sidesNeighbours){
      Cell c = neighbours.get(key);
      if(c == null || c.data == 0)
        return false;
    }
    return true;
  }

  /**
   *
   * @param direction Key of a neighbour that induce the direction of the exploration
   * @param distance Number of step of the exploration. Supposed to be >= 1
   * @return the data of the distance-th cell with respect of direction or 0 if these cell doesn't exist
   */
  public int exploreNeighbours(int direction, int distance){
    Cell current = this;
    for(int  x = 1; x <= distance; x++){
      current = current.neighbours.get(direction);
      if(current == null)
        return 0;
    }
    return current.data;
  }

  public int exploreColor(int direction, int distance){
    Cell current = this;
    for(int  x = 1; x <= distance; x++){
      current = current.neighbours.get(direction);
      if(current == null)
        return 0;
    }
    return current.color;
  }

  public HashSet<Cell> ball(int radius){
    HashSet<Cell> toReturn = new HashSet<>();
    HashMap<Cell, Integer> dist  = new HashMap<>();
    Queue<Cell> q = new LinkedList<>();
    q.add(this);
    dist.put(this, 0);

    while(!(q.isEmpty())){

      Cell current = q.poll();

      if(dist.get(current) < radius){
        for(Cell c : current.neighbours.values()){
          dist.putIfAbsent(c, dist.get(current) + 1);
          if(!toReturn.contains(c) && !q.contains(c))
            q.add(c);
        }
      }

      toReturn.add(current);
    }
    toReturn.remove(this);
    return toReturn;

  }

  public HashSet<Cell> sphere(int radius){
    HashSet<Cell> toReturn = new HashSet<>();
    HashSet<Cell> visited  = new HashSet<>();
    HashMap<Cell, Integer> dist  = new HashMap<>();
    Queue<Cell> q = new LinkedList<>();
    q.add(this);
    dist.put(this, 0);

    while(!(q.isEmpty())){
      Cell current = q.poll();

      if(dist.get(current) < radius){
        for(Cell c : current.neighbours.values()){
          dist.putIfAbsent(c, dist.get(current) + 1);
          if(!visited.contains(c) && !q.contains(c))
            q.add(c);
        }
      }
      else
        toReturn.add(current);
      visited.add(current);
    }
    return toReturn;

  }
  @Override
  public String toString(){
    return "Cell{data: " + data + " color: "+ color +"}";
  }
}
