package util;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TwoDWord<T extends Alphabet>{

  private ArrayList<ArrayList<T>> word;

  public TwoDWord(ArrayList<ArrayList<T>> word){

    this.word = word;
  }

  public TwoDWord(T chr){
    ArrayList<T> initChr = new ArrayList<>();
    initChr.add(chr);
    word = new ArrayList<>(Collections.singletonList(initChr));
  }

  public T get(int i, int j){
    return word.get(i).get(j);
  }

  public int getHeight(){
    return word.size();
  }

  public int getWidth(){
    return word.get(0).size();
  }

  public ArrayList<ArrayList<T>> getWord(){
    ArrayList<ArrayList<T>> toReturn = new ArrayList<>();
    for(ArrayList<T> row : word)
      toReturn.add(new ArrayList<>(row));
    return toReturn;
  }

  public void applyHorizontalChanges(int i, int j, List<T> changes){
    word.get(i).remove(j);
    word.get(i).addAll(j, changes);
  }

  public void applyVerticalChanges(int i, List<ArrayList<T>> changes){
    word.remove(i);
    word.addAll(i, changes);
  }

  public boolean equals(TwoDWord<T> other){

    for(int i = 0; i < word.size(); i++){
      for(int j = 0; j < word.size(); j++){
        if(this.get(i, j) != other.get(i, j))
          return false;
      }
    }
    return true;
  }

  @Override
  public String toString(){
    StringBuilder toReturn = new StringBuilder();
    for(ArrayList<T> row : word){
      for(T chr : row){
        toReturn.append(chr);
        toReturn.append(" ");
      }
      toReturn.append("\n");
    }
    return toReturn.toString();
  }
}
