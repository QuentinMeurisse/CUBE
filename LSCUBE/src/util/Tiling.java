package util;

import java.util.*;

public abstract class Tiling implements Iterable<Cell>{

  protected ArrayList<Cell> cells = new ArrayList<>(5000);
  protected double totalArea;
  protected double outerPermiter;
  protected TwoDWord<? extends Alphabet> word; //Use for the graphical representation
  protected int width;
  protected int height;
  protected double sideLength;

  /**
   * Use for the graphical representation
   * @param i
   * @param j
   * @return the data of the cell corresponding to the character (i, j)
   */
  public abstract int getColorOfCorrespondingLetter(int i, int j);

  public abstract boolean equals(Tiling other);

  public abstract double getOuterPerimeter();

  public abstract double maxPerimeter();

  /**
   * Method which generate all the cells of the tiling
   * and the 2D word representing the tiling
   */
  protected abstract void generate();

  /**
   *
   * @param cells
   * @return a new instance of a concrete tiling. Use during a copy
   */
  protected abstract Tiling newInstance(ArrayList<Cell> cells);

  public int numCells(){
    return cells.size();
  }

  public double getTotalArea(){
    return totalArea;
  }

  public Cell getCell(int i){
    return cells.get(i);
  }

  public TwoDWord<? extends Alphabet> getWord(){
    return word;
  }

  public int getWidth(){
    return width;
  }

  public int getHeight(){
    return height;
  }

  public int indexOf(Cell c){
    return cells.indexOf(c);
  }

  public HashMap<Cell, Cell> mapForClone(){
    Cell source = cells.get(0);
    Queue<Cell> q = new LinkedList<>(); //Cells having neighbours that must be cloned
    q.add(source);

    HashMap<Cell, Cell> hm = new HashMap<>(); //Map that link a cell and its clone
    Cell sourceCopy = new Cell(source.getNumSides(),
            source.getNumNeighbours(),
            source.getSidesNeighbours().clone(),
            source.getData(),
            source.getSideLength());
    sourceCopy.setColor(source.getColor());
    if(source.isLocked())
      sourceCopy.lock();
    else
      sourceCopy.unlock();
    hm.put(source, sourceCopy);

    while(!q.isEmpty()){

      Cell c = q.poll();

      Cell cloneC = hm.get(c); //A cell in the queue is already cloned

      if(!(c.getNeighbours().isEmpty())){
        HashMap<Integer, Cell> cNeighbours = c.getNeighbours();
        for(int key : cNeighbours.keySet()){

          Cell x = cNeighbours.get(key);
          Cell cloneX = hm.get(x);
          if(cloneX == null){ //if x is not already cloned
            q.add(x);
            cloneX = new Cell(x.getNumSides(),
                    x.getNumNeighbours(),
                    x.getSidesNeighbours().clone(),
                    x.getData(),
                    x.getSideLength());
            cloneX.setColor(x.getColor());
            if(x.isLocked())
              cloneX.lock();
            else
              cloneX.unlock();
            hm.put(x, cloneX);
          }
          cloneC.addNeighbour(key, cloneX); // we add the clone of x as a neighbour of the clone of c
        }
      }
    }
    return hm;
  }

  public Tiling clone(HashMap<Cell, Cell> hm){
    //TODO: améliorer la copie ?
    ArrayList<Cell> clonedCells = new ArrayList<>();
    for(Cell cell : cells)
      clonedCells.add(hm.get(cell));

    return newInstance(clonedCells);
  }

  @Override
  public Iterator<Cell> iterator(){
    return cells.iterator();
  }

  @Override
  public String toString(){
    StringBuilder s = new StringBuilder();
    int k = 0;
    for(Cell c : cells){
      s.append("Cell num : ").append(k).append(" ");
      s.append(c.toString());
      s.append(" ");
      s.append(c.getNeighbours());
      s.append("\n");
      k++;
    }
    return s.toString() + "\n" + word.toString();

  }
}