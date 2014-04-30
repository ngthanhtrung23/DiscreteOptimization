// Compile: javac -classpath "gurobi.jar:." SolutionGurobi2.java
// Run:     java -classpath "gurobi.jar:." SolutionGurobi2

import gurobi.*;
import java.util.*;
import java.io.*;

public class SolutionGurobi2
{
    public static final String INPUT_FILE = "./data/tsp_51_1";
    public static final String OUTPUT_FILE = "output_1.txt";

    public static void main(String[] args) {
        (new Solver()).go();
    }
}

class Solver
{
    int n;
    double[] x, y;

    GRBEnv  env;
    GRBModel model;
    GRBVar[][] vars;

    public void buildModel() throws Exception {
        env = new GRBEnv();
        model = new GRBModel(env);
        vars = new GRBVar[n][n];

        // x[i][j] = 1 iff j appears after i
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                double dx = x[i] - x[j];
                double dy = y[i] - y[j];
                vars[i][j] = model.addVar(0.0, (i == j) ? 0.0 : 1.0, Math.sqrt(dx*dx + dy*dy),
                        GRB.BINARY,
                        i + "_" + j);
            }
        // Integrate variables
        model.update();

        // Add constraints
        // Each node is followed by exactly 1 node
        for(int i = 0; i < n; ++i) {
            GRBLinExpr expr = new GRBLinExpr();
            for(int j = 0; j < n; j++) {
                expr.addTerm(1.0, vars[i][j]);
            }
            model.addConstr(expr, GRB.EQUAL, 1.0, "");
        }
        // Each node is only after exactly 1 node
        for(int j = 0; j < n; ++j) {
            GRBLinExpr expr = new GRBLinExpr();
            for(int i = 0; i < n; ++i) {
                expr.addTerm(1.0, vars[i][j]);
            }
            model.addConstr(expr, GRB.EQUAL, 1.0, "");
        }
        model.update();
    }

    public void readInput() {
        try {
            Scanner sc = new Scanner(new File(SolutionGurobi2.INPUT_FILE));
            n = sc.nextInt();
            x = new double[n];
            y = new double[n];

            for(int i = 0; i < n; ++i) {
                x[i] = sc.nextInt();
                y[i] = sc.nextInt();
            }
        }
        catch (IOException ex) {
            ex.printStackTrace();
            System.out.println("Cannot read input");
            System.exit(0);
        }
    }

    public void initialize() throws Exception {
        try {
            Scanner sc = new Scanner(new File(SolutionGurobi2.OUTPUT_FILE));
            double cost = sc.nextDouble();
            int tmp = sc.nextInt();

            int[] path = new int[n];
            for(int i = 0; i < n; ++i) {
                path[i] = sc.nextInt();
            }
            for(int i = 0; i < n; ++i) {
                int j = (i+1) % n;
                vars[i][j].set(GRB.DoubleAttr.Start, 1.0);
            }
        }
        catch (Exception ex) {
            ex.printStackTrace();
            System.out.println("Cannot init solution");
            System.exit(0);
        }
        model.getEnv().set(GRB.IntParam.Method, GRB.METHOD_BARRIER);
        model.getEnv().set(GRB.DoubleParam.TimeLimit, 100);
    }

    public void go() {
        try {
            readInput();
            buildModel();
            initialize();
            model.optimize();

            System.out.println(model.get(GRB.DoubleAttr.ObjVal) + " 0");
            if (model.get(GRB.IntAttr.SolCount) > 0) {
                int u = 0;
                for(int turn = 0; turn < n; ++turn) {
                    System.out.print(u + " ");
                    for(int v = 0; v < n; ++v)
                        if (vars[u][v].get(GRB.DoubleAttr.X) == 1.0) {
                            u = v;
                            break;
                        }
                }
                System.out.println();
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
