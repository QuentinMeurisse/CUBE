package util;

import java.util.HashSet;
import java.util.Set;

public class SetOperations{

  public static <T> Set<T> union(Set<T> s1, Set<T> s2){
    HashSet<T> toReturn = new HashSet<>();
    toReturn.addAll(s1);
    toReturn.addAll(s2);
    return toReturn;
  }

  public static <T> Set<T> intersection(Set<T> s1, Set<T> s2){
    HashSet<T> toReturn = new HashSet<>();
    Set<T> smaller = s1.size() < s2.size() ? s1 : s2;
    Set<T> larger = s1.size() < s2.size() ? s2 : s1;
    for(T x : smaller){
      if(larger.contains(x))
        toReturn.add(x);
    }
    return toReturn;
  }

  public static <T> Set<T> difference(Set<T> s1, Set<T> s2){
    HashSet<T> toReturn = new HashSet<>();
    for(T x : s1){
      if(!s2.contains(x))
        toReturn.add(x);
    }
    return toReturn;
  }
}
