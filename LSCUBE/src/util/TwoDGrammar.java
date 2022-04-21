package util;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

public class TwoDGrammar<T extends Alphabet>{

  private ArrayList<Function<T, List<T>>> horizontalRules;
  private ArrayList<Function<T, List<T>>> verticalRules;

  public TwoDGrammar(ArrayList<Function<T, List<T>>> horizontalRules,
                     ArrayList<Function<T, List<T>>> verticalRules){
    this.horizontalRules = horizontalRules;
    this.verticalRules = verticalRules;
  }

  public List<T> applyHorizontalRules(int i, T chr){
    Function<T, List<T>> f = horizontalRules.get(i);
    return f.apply(chr);
  }

  public List<T> applyVerticalRules(int i, T chr){
    Function<T, List<T>> f = verticalRules.get(i);
    return f.apply(chr);
  }
}
