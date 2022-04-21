package util;

public class TooMuchNeighbourException extends RuntimeException{

  public TooMuchNeighbourException(){
  }

  public TooMuchNeighbourException(String s){
    super(s);
  }
}
