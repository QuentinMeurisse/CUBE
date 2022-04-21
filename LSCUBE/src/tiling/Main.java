package tiling;
import java.awt.*;
import java.awt.event.WindowEvent;
import java.math.BigDecimal;
import java.util.Locale;
import java.util.Scanner;
import javax.swing.JFrame;
import javax.swing.JOptionPane;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.jamesframework.core.search.algo.ParallelTempering;
import org.jamesframework.core.search.algo.SteepestDescent;
import org.jamesframework.core.search.algo.RandomDescent;
import tiling.constraint.*;
import tiling.gui.*;
import tiling.neighbourhood.ChangeColorAndMergeNeighbourhood;
import tiling.neighbourhood.ChangeColorMove;
import tiling.neighbourhood.ChangeColorNeighbourhood;
import tiling.neighbourhood.GreenSpaceInBuildingNeighbourhood;
import tiling.othercomponent.ExportSquareListener;
import tiling.othercomponent.GreenSpacesListener;
import tiling.othercomponent.RandomUrbanBlockGenerator;
import tiling.othercomponent.TilingListener;
import tiling.solution.*;
import org.jamesframework.core.problems.GenericProblem;
import org.jamesframework.core.search.stopcriteria.MaxTimeWithoutImprovement;
import tiling.objective.PorosityObjective;
import util.*;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.TimeUnit;
import java.util.function.Function;

public class Main {
    public static void main(String[] args) {
        Logger.getRootLogger().setLevel(Level.OFF);
        JFrame askFrame = new JFrame();
        askFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        try {


            String tilingType = JOptionPane.showInputDialog(askFrame, "Choose type of tiling: square, triangular or hexagonal");
            while (!tilingType.equals("square") && !tilingType.equals("triangular") && !tilingType.equals("hexagonal")) {
                tilingType = JOptionPane.showInputDialog(askFrame, "Bad entry\nChoose type of tiling: square, triangular or hexagonal");
            }

            int w = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "What is the width of the tiling? (enter a integer greater than 0)"));
            int h = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "What is the height of the tiling? (enter a integer greater than 0)"));

            double sl = Double.parseDouble(JOptionPane.showInputDialog(askFrame, "What is the length of a side of a tile? (enter a number greater than 0)"));

            HashMap<Integer, Integer> toLock = new HashMap<>();

            int lock = JOptionPane.showConfirmDialog(askFrame, "Do you want to lock a cell?");
            while (lock == JOptionPane.YES_OPTION) {
                int x = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter a number in [0," + h * w + "["));
                toLock.put(x, 20);
                lock = JOptionPane.showConfirmDialog(askFrame, "Do you want to lock a cell?");
            }

            JOptionPane.showMessageDialog(askFrame, "Let's define the problem");
            PorosityObjective obj = new PorosityObjective();
            UrbanBlockData data = new UrbanBlockData();

            int use_green = JOptionPane.showConfirmDialog(askFrame, "Do you want to consider green spaces?");
            ArrayList<Function<UrbanBlockSolution, UrbanBlockInvariant>> invariants = new ArrayList<>();
            invariants.add(BuildingInvariant::new);
            if (use_green == JOptionPane.YES_OPTION)
                invariants.add(GreenSpacesInvariant::new);

            TriFunction<Integer, Integer, Double, Tiling> initTiling;
            if (tilingType.equals("square"))
                initTiling = SquareTiling::new;
            else if (tilingType.equals("triangular"))
                initTiling = TriangularTiling::new;
            else
                initTiling = HexagonalTiling::new;

            ChangeColorNeighbourhood neighbourhood = new ChangeColorNeighbourhood(0);

            RandomUrbanBlockGenerator rng = new RandomUrbanBlockGenerator(500, TimeUnit.MILLISECONDS,
                    w, h, sl, initTiling, toLock, invariants, neighbourhood);

            GenericProblem<UrbanBlockSolution, UrbanBlockData> problem = new GenericProblem<>(data, obj, rng);

            int use_constraint = JOptionPane.showConfirmDialog(askFrame, "Do you want to constraint the distance between the facades?");
            if (use_constraint == JOptionPane.YES_OPTION) {
                int r = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter the radius (an integer)."));
                problem.addMandatoryConstraint(new RadiusConstraint(r));
            }

            int nb = JOptionPane.showConfirmDialog(askFrame, "Do you want to constraint the number of buildings?");
            if (nb == JOptionPane.YES_OPTION) {
                int minB = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter the minimum number of buildings (an integer)."));
                int maxB = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter the maximum number of buildings (an integer)."));
                problem.addPenalizingConstraint(new NumBuildingConstraint(minB, maxB, false));

            }

            int pb = JOptionPane.showConfirmDialog(askFrame, "Do you want to constraint the perimeter of buildings?");
            if (pb == JOptionPane.YES_OPTION) {
                int minP = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter the minimum perimeter of buildings (an integer)."));
                int maxP = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter the maximum perimeter of buildings (an integer)."));
                problem.addPenalizingConstraint(new BuildingsPerimeterConstraint(minP, maxP, true));
            }

            if (use_green == JOptionPane.YES_OPTION) {
                double green_rate = Double.parseDouble(JOptionPane.showInputDialog(askFrame, "What percentage of the surface should be occupied by green spaces? Type a number in [0, " + 1 + "]"));
                problem.addPenalizingConstraint(new GreenSpacesConstraint(green_rate));
            }

            String meta = "Choose a metaheuristic:\n- Hill Climbing (type hc),\nRandom Descent (type rd),\n- Parallel Tempering (type pt).";
            String metaheuristic = JOptionPane.showInputDialog(askFrame, meta).toLowerCase();

            if (metaheuristic.equals("hc")) {
                JOptionPane.showMessageDialog(askFrame, "The search will start.");
                TilingPanel panel;
                if (tilingType.equals("square")) {
                    SquareTiling t = new SquareTiling(w, h, sl);
                    panel = new SquarePanel(t);
                } else if (tilingType.equals("triangular")) {
                    TriangularTiling t = new TriangularTiling(w, h, sl);
                    panel = new TrianglePanel(t);
                } else {
                    HexagonalTiling t = new HexagonalTiling(w, h, sl);
                    panel = new HexagonPanel(t);
                }
                TilingFrame frame = new TilingFrame(panel);
                SteepestDescent<UrbanBlockSolution> hc = new SteepestDescent<>(problem, new ChangeColorNeighbourhood(-1 * (invariants.size() - 1)));
                hc.addSearchListener(new TilingListener(frame));
                hc.start();
                hc.dispose();
            } else if (metaheuristic.equals("rd")) {
                int max_time = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter max time without improvement (in ms): "));
                JOptionPane.showMessageDialog(askFrame, "The search will start.");

                TilingPanel panel;
                if (tilingType.equals("square")) {
                    SquareTiling t = new SquareTiling(w, h, sl);
                    panel = new SquarePanel(t);
                } else if (tilingType.equals("triangular")) {
                    TriangularTiling t = new TriangularTiling(w, h, sl);
                    panel = new TrianglePanel(t);
                } else {
                    HexagonalTiling t = new HexagonalTiling(w, h, sl);
                    panel = new HexagonPanel(t);
                }
                TilingFrame frame = new TilingFrame(panel);

                RandomDescent<UrbanBlockSolution> rd = new RandomDescent<>(problem, new ChangeColorNeighbourhood(-1 * (invariants.size() - 1)));
                rd.addStopCriterion(new MaxTimeWithoutImprovement(max_time, TimeUnit.MILLISECONDS));
                rd.addSearchListener(new TilingListener(frame));
                rd.start();
                rd.dispose();
            } else {
                int max_time = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter max time without improvement (in ms):"));
                int numReplicas = Integer.parseInt(JOptionPane.showInputDialog(askFrame, "Enter the number of replicas (an integer):"));
                BigDecimal bdminT = new BigDecimal(JOptionPane.showInputDialog(askFrame, "Enter the minimum temperature of the system: "));
                double minT = bdminT.doubleValue();
                BigDecimal bdMaxT = new BigDecimal(JOptionPane.showInputDialog(askFrame, "Enter the maximum temperature of the system: "));
                double maxT = bdMaxT.doubleValue();
                JOptionPane.showMessageDialog(askFrame, "The search will start.");

                TilingPanel panel;
                if (tilingType.equals("square")) {
                    SquareTiling t = new SquareTiling(w, h, sl);
                    panel = new SquarePanel(t);
                } else if (tilingType.equals("triangular")) {
                    TriangularTiling t = new TriangularTiling(w, h, sl);
                    panel = new TrianglePanel(t);
                } else {
                    HexagonalTiling t = new HexagonalTiling(w, h, sl);
                    panel = new HexagonPanel(t);
                }
                TilingFrame frame = new TilingFrame(panel);
                ParallelTempering<UrbanBlockSolution> pt = new ParallelTempering<>(problem,
                        new ChangeColorAndMergeNeighbourhood(2, -1 * (invariants.size() - 1)),
                        numReplicas, minT, maxT);
                pt.addStopCriterion(new MaxTimeWithoutImprovement(max_time, TimeUnit.MILLISECONDS));
                pt.addSearchListener(new TilingListener(frame));
                pt.start();
                pt.dispose();
            }
        } catch (Exception e){
            JOptionPane.showMessageDialog(askFrame, "Error dectected");
            askFrame.dispatchEvent(new WindowEvent(askFrame, WindowEvent.WINDOW_CLOSING));

        }
    }
}
