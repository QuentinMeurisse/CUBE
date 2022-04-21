package util;

import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class Utils{

  public static Stack<Integer> copyStack(Stack<Integer> s){
    Object[] tab = new Object[s.size()];
    ArrayList<Integer> tmp = new ArrayList<>();
    s.copyInto(tab);
    for(Object o : tab)
      tmp.add((int) o);

    Stack<Integer> s2 = new Stack<>();
    s2.addAll(tmp);
    return s2;
  }

  /**
   *
   * @param domain a collection of integer supposed to be sorted
   * @param n
   * @return all the n-tuple from the number in domain without symmetry and tuple (x, x)
   */
  public static ArrayList<ArrayList<Integer>> generateNTuples(Collection<Integer> domain, int n){
    if(n == 0) // A 0-uple is just a empty list
      return new ArrayList(Arrays.asList(new ArrayList()));
    else{
      //To a have a n-tuple, we just add a component to the (n-1)-tuples.
      // So, we compute them with a recursive call
      ArrayList<ArrayList<Integer>> t = generateNTuples(domain, n - 1);
      ArrayList<ArrayList<Integer>> toReturn = new ArrayList<>();
      for(int i : domain){ //for all the value in our range
        for(ArrayList<Integer> s : t){ //for all the (n-1)-tuples
          ArrayList<Integer> c = new ArrayList<>(Arrays.asList(i));
          if(s.size() == 0 || i > s.get(s.size()- 1)){
            //We only create a n-tuple if n == 1 or if i is greater than the last component
            // of our current (n-1)-tuple. The last condition stands to avoid symmetry.
            List<Integer> w = Stream.of(s, c) //Create a stream from the current (n-1)-tuple and [i]
                    .flatMap(List::stream) //Return a stream first from s and a stream from [i]
                    .collect(Collectors.toList()); //Create a list from and all element of s and add i at end

            toReturn.add(new ArrayList<>(w));
          }
        }
      }
      return toReturn;
    }
  }

  /**
   *
   * @param from
   * @param to
   * @param n
   * @return all the n-tuple from the number in range [from, to[ without symmetry and tuple (x, x)
   */
  public static ArrayList<ArrayList<Integer>> generateNTuples(int from, int to, int n){
    List<Integer> t  = IntStream.range(from, to).boxed().collect(Collectors.toList());
    return generateNTuples(t, n);
  }

  /**
   *
   * @param first
   * @param domain
   * @param n
   * @return all the n-tuple from the number in domain without symmetry and tuple (x, x) with first as first component
   */
  public static ArrayList<ArrayList<Integer>> generateNTuples(int first, Collection<Integer> domain, int n){
    List<Integer> t = domain.stream().filter( x -> x != first).collect(Collectors.toList());
    ArrayList<ArrayList<Integer>> tuples = generateNTuples(t, n-1);
    ArrayList<ArrayList<Integer>> toReturn = new ArrayList<>();
    ArrayList<Integer> c = new ArrayList<>(Arrays.asList(first));
    for(ArrayList<Integer> tab : tuples){
      List<Integer> w = Stream.of(c, tab)
              .flatMap(List::stream)
              .collect(Collectors.toList());

      toReturn.add(new ArrayList<>(w));
    }
    return toReturn;
  }

  public static ArrayList<ArrayList<Integer>> generateNTuples(int first,int from, int to, int n){
    List<Integer> t  = IntStream.range(from, to).boxed().collect(Collectors.toList());
    return generateNTuples(first, t, n);
  }
}
