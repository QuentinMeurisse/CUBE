package tiling.solution;

import util.Cell;

import java.util.HashMap;
import java.util.HashSet;

public class GroupOfCells{

  protected final int id;

  protected int color;
  protected HashSet<Cell> border = new HashSet<>();
  protected HashSet<Cell> inner = new HashSet<>();
  protected double area;
  protected double perimeter;



  public GroupOfCells(int id, Cell c){
    this.id = id;
    this.color = c.getColor();
    border.add(c);
    area = c.getArea();
    perimeter = c.getPerimeter();
  }

  public GroupOfCells(int id){
    this.id = id;
    area = 0;
    perimeter = 0;
  }

  public void addCell(Cell c){
    c.setData(id);
    c.setColor(color);
    if(c.areAllNeighboursFilled())
      inner.add(c);
    else
      border.add(c);
    for(Cell d : c.getNeighbours().values()){
      if(d.areAllNeighboursFilled() && border.remove(d)){
        inner.add(d);
      }
    }
    area += c.getArea();
    for(int key : c.getSidesNeighbours()){
      Cell neigh = c.getNeighbour(key);
      if(neigh == null || perimeterCondition(neigh.getColor()))
        perimeter += c.getSideLength();
      else
        perimeter -= c.getSideLength();
    }

  }

  public void removeCell(Cell c){
    if(border.remove(c) || inner.remove(c)){
      area -= c.getArea();
      c.setData(0);
      c.setColor(0);
      for(int key : c.getSidesNeighbours()){
        Cell neigh = c.getNeighbour(key);
        if(isInnerCell(neigh)){
          inner.remove(neigh);
          border.add(neigh);
        }
        if(neigh == null || perimeterCondition(neigh.getColor()))
          perimeter -= c.getSideLength();
        else
          perimeter += c.getSideLength();
      }
    }
  }

  public void changeColor(int color){
    this.color = color;
    for(Cell c : border){
      c.setColor(color);
      if(color == 0)
        c.setData(0);
      else
        c.setData(id);
    }
    for(Cell c : inner){
      c.setColor(color);
      if(color == 0)
        c.setData(0);
      else
        c.setData(id);
    }
  }

  public int getId(){
    return id;
  }

  public int getColor(){
    return color;
  }

  public double getArea(){
    return area;
  }

  public double getPerimeter(){
    return perimeter;
  }

  public HashSet<Cell> getBorder(){
    return border;
  }

  public GroupOfCells copy(HashMap<Cell, Cell> hm){
    GroupOfCells cloned = new GroupOfCells(id);
    cloned.perimeter = this.perimeter;
    cloned.area = this.area;
    cloned.color = this.color;
    for(Cell c : this.border)
      cloned.border.add(hm.get(c));
    for(Cell c : this.inner)
      cloned.inner.add(hm.get(c));
    return cloned;
  }

  public boolean isBorderCell(Cell c){
    return border.contains(c);
  }

  public boolean isInnerCell(Cell c){
    return inner.contains(c);
  }

  public boolean contains(Cell c){
    return isBorderCell(c) || isInnerCell(c);
  }

  public void correctCellsData(){
    for(Cell c : border)
      c.setData(id);
    for(Cell c : inner)
      c.setData(id);
  }

  private boolean perimeterCondition(int cellColor){
    if(color > 0)
      return cellColor <= 0;
    else{
      return cellColor != color;
    }
  }
}
