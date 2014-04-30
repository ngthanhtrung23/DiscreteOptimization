// Compile: javac -classpath "gurobi.jar:." SolutionGurobi.java
// Run:     java -classpath "gurobi.jar:." SolutionGurobi

import gurobi.*;
import java.util.*;
import java.io.*;

public class SolutionGurobi
{
    public static final String INPUT_FILE = "./data/tsp_51_1";
    public static final String OUTPUT_FILE = "output.txt";
    public static final Double BEST_SOLUTION = 469.755;

    public static void main(String[] args) {
        (new Solver()).go();
    }
}

class Solver extends GRBCallback
{
    private final double threshold = 10000000.0;
    private GRBVar[] vars;
    int[][] id;
    int nVar;

    public Solver(GRBVar[] xvars) {
        vars = xvars;
    }
    public Solver() {
    }

    // Subtour elimination callback.  Whenever a feasible solution is found,
    // find the subtour that contains node 0, and add a subtour elimination
    // constraint if the tour doesn't visit every node.
    protected void callback() {
        try {
            if (where == GRB.CB_MIPSOL) {
                // Found an integer feasible solution - does it visit every node?
                int n = vars.length;
                int[] tour = findsubtour(getSolution(vars));

                if (tour.length < n) {
                    // Add subtour elimination constraint
                    GRBLinExpr expr = new GRBLinExpr();
                    for (int i = 0; i < tour.length-1; i++)
                        if (id[tour[i]][tour[i+1]] >= 0)
                            expr.addTerm(1.0, vars[id[tour[i]][tour[i+1]]]);
                    if (id[tour[tour.length-1]][tour[0]] >= 0)
                        expr.addTerm(1.0, vars[id[tour[tour.length-1]][tour[0]]]);
                    addLazy(expr, GRB.LESS_EQUAL, tour.length-1);
                }
            }
        }
        catch (GRBException e) {
            e.printStackTrace();
        }
    }

    // Given an integer-feasible solution 'sol', find the sub-tour that
    // contains node 0.  Result is returned in 'tour', and length is
    // returned in 'tourlenP'.
    protected int[] findsubtour(double[] sol) {
        boolean[] seen = new boolean[n];
        int[] tour = new int[n];
        int i, index, node;

        for (i = 0; i < n; i++)
            seen[i] = false;

        node = 0;
        for (index = 0; index < n; index++) {
            tour[index] = node;
            seen[node] = true;
            for (i = 0; i < n; i++)
                if (id[node][i] >= 0 && sol[id[node][i]] > 0.5 && !seen[i]) {
                    node = i;
                    break;
                }
            if (i == n)
                break;
        }
        int result[] = new int[index+1];
        for (i = 0; i <= index; i++)
            result[i] = tour[i];
        return result;
    }

    int n;
    double[] x, y;

    public void go() {
        try {
            readInput();
            solve();
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    double distance(int i, int j) {
        double dx = x[i] - x[j];
        double dy = y[i] - y[j];
        return Math.sqrt(dx*dx + dy*dy);
    }

    void solve() throws Exception {
        GRBEnv  env   = new GRBEnv();
        GRBModel model = new GRBModel(env);
        // Must disable dual reductions when using lazy constraints
        model.getEnv().set(GRB.IntParam.DualReductions, 0);

        // Create variables
        int cnt = 0;
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if (distance(i, j) < threshold) {
                    ++cnt;
                }
            }
        }
        GRBVar[] vars = new GRBVar[cnt];
        id = new int[n][n];

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (distance(i, j) < threshold) {
                    id[i][j] = nVar++;
                    vars[id[i][j]] = model.addVar(0.0, 1.0, distance(i, j),
                            GRB.BINARY,
                            "x"+String.valueOf(i)+"_"+String.valueOf(j));
                }
                else {
                    id[i][j] = -1;
                }

        // Integrate variables
        model.update();

        // Degree-2 constraints
        for (int i = 0; i < n; i++) {
            GRBLinExpr expr = new GRBLinExpr();
            for (int j = 0; j < n; j++)
                if (id[i][j] >= 0) {
                    expr.addTerm(1.0, vars[id[i][j]]);
                }
            model.addConstr(expr, GRB.EQUAL, 2.0, "deg2_"+String.valueOf(i));
        }

        // Forbid edge from node back to itself
        for (int i = 0; i < n; i++)
            if (id[i][i] >= 0) {
                vars[id[i][i]].set(GRB.DoubleAttr.UB, 0.0);
            }

        // Symmetric TSP
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                if (id[i][j] >= 0)
                    model.addConstr(vars[id[i][j]], GRB.EQUAL, vars[id[j][i]], "");


        // Read initial values
        try {
            Scanner sc = new Scanner(new File("output_1_backup.txt"));
            double cost = sc.nextDouble();
            int tmp = sc.nextInt();

            for(int i = 0; i < n; ++i) {
                int u = sc.nextInt();
                if (id[i][u] >= 0)
                    vars[id[i][u]].set(GRB.DoubleAttr.Start, 1.0);
            }
        }
        catch (IOException ex) {
            System.out.println(":@)");
        }
        System.out.println("OK");

        model.getEnv().set(GRB.IntParam.Method, GRB.METHOD_BARRIER);
        model.getEnv().set(GRB.DoubleParam.TimeLimit, 100);

        model.getEnv().set(GRB.DoubleParam.Cutoff, SolutionGurobi.BEST_SOLUTION * 2.0);
        model.setCallback(new Solver(vars));
        model.optimize();

        System.out.println(model.get(GRB.DoubleAttr.ObjVal) / 2.0 + " 0");
        if (model.get(GRB.IntAttr.SolCount) > 0) {
            int[] tour = findsubtour(model.get(GRB.DoubleAttr.X, vars));
            for (int i = 0; i < tour.length; i++)
                System.out.print(String.valueOf(tour[i]) + " ");
            System.out.println();
        }

        model.dispose();
        env.dispose();
    }

    void readInput() throws Exception {
        Scanner sc = new Scanner(new File(SolutionGurobi.INPUT_FILE));
        n = sc.nextInt();
        System.out.println("n = " + n);
        x = new double[n];
        y = new double[n];
        for(int i = 0; i < n; ++i) {
            x[i] = sc.nextDouble();
            y[i] = sc.nextDouble();
        }
        sc.close();
    }
}
