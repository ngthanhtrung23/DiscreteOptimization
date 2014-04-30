// Compile: javac -classpath "gurobi.jar:." Solution.java
// Run: java -classpath "gurobi.jar:." Solution

import gurobi.*;
import java.io.*;
import java.util.*;
import java.math.*;

public class Solution
{
    public static void main(String[] args) throws Exception {
        (new Solver()).go();
    }
}

class Solver
{
    final int NFACILITY = 250;
    int nFacility, nConsumer;
    double[] setupCost, allSetupCost;
    double[] capacity, allCapacity;
    double[] demand;
    double[] x_facility, y_facility, x_consumer, y_consumer;
    double[][] transport;

    double capacityLimit, setupCostLimit;

    public void go() throws Exception {
        read();
        // Create model
        GRBEnv env = new GRBEnv();
        GRBModel model = new GRBModel(env);
        model.set(GRB.StringAttr.ModelName, "Coursera - Discrete Optimization - Facility");

        // Facility open decision variables: open[f] == 1 if facility f is open.
        GRBVar[] open = new GRBVar[nFacility];
        for (int f = 0; f < nFacility; ++f)
            if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                open[f] = model.addVar(0, 1, setupCost[f], GRB.BINARY, "Open" + f);
            }

        // Transportation decision variables: go[i][j] == 1 if
        // we transport from facility i to consumer j
        GRBVar[][] go = new GRBVar[nFacility][nConsumer];
        for (int f = 0; f < nFacility; ++f)
            if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                for(int c = 0; c < nConsumer; ++c) {
                    go[f][c] = model.addVar(0, 1,
                        transport[f][c],
                        GRB.BINARY,
                        "Transport " + f + "-" + c
                        );
                }
            }
        // The objective is to minimize the total fixed and variable costs
        GRBLinExpr obj = new GRBLinExpr();
        for(int f = 0; f < nFacility; ++f)
            if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                obj.addTerm(setupCost[f], open[f]);
            }
        for(int f = 0; f < nFacility; ++f) {
            if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                for(int c = 0; c < nConsumer; ++c)
                    obj.addTerm(transport[f][c], go[f][c]);
            }
        }
        model.update();
        model.setObjective(obj, GRB.MINIMIZE);

        // Update model to integrate new variables
        model.update();

        // Production constraints
        for(int f = 0; f < nFacility; ++f) {
            if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                GRBLinExpr ftot = new GRBLinExpr();
                for (int c = 0; c < nConsumer; ++c) {
                    ftot.addTerm(demand[c], go[f][c]);
                }
                GRBLinExpr limit = new GRBLinExpr();
                limit.addTerm(capacity[f], open[f]);
                model.addConstr(ftot, GRB.LESS_EQUAL, limit, "Capacity" + f);
            }
        }

        // Demand constraints
        for(int c = 0; c < nConsumer; ++c) {
            GRBLinExpr ctot = new GRBLinExpr();
            for(int f = 0; f < nFacility; ++f) {
                if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                    ctot.addTerm(1.0, go[f][c]);
                }
            }
            model.addConstr(ctot, GRB.EQUAL, 1.0, "Demand" + c);
        }

        // For test 4:
        if (true) {
           try {
                Scanner sc = new Scanner(new File("output_6.txt"));
                double cost = sc.nextDouble();
                int tmp = sc.nextInt();

                boolean[] isOpened = new boolean[nFacility];
                for(int i = 0; i < nFacility; ++i)
                    isOpened[i] = false;

                for(int i = 0; i < nConsumer; ++i) {
                    int u = sc.nextInt();
                    if (setupCost[u] <= setupCostLimit) {
                        if (!isOpened[u]) {
                            isOpened[u] = true;
                            open[u].set(GRB.DoubleAttr.Start, 1.0);
                        }
                        go[u][i].set(GRB.DoubleAttr.Start, 1.0);
                    }
                }
            }
            catch (IOException ex) {
                System.out.println(":@)");
            }
         }
        else {
            // First, open all plants
            for (int f = 0; f < nFacility; ++f) {
                if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                    open[f].set(GRB.DoubleAttr.Start, 1.0);
                }
            }
        }

        // Use barrier to solve root relaxation
        model.getEnv().set(GRB.IntParam.Method, GRB.METHOD_BARRIER);
        model.getEnv().set(GRB.DoubleParam.TimeLimit, 200);

        // Solve
        model.optimize();

        // Print solution
        System.out.println(model.get(GRB.DoubleAttr.ObjVal) + " 0");
        for(int c = 0; c < nConsumer; ++c) {
            int found = -1;
            for(int f = 0; f < nFacility; ++f) {
                if (capacity[f] > capacityLimit && setupCost[f] <= setupCostLimit) {
                    if (go[f][c].get(GRB.DoubleAttr.X) == 1.0) {
                        found = f;
                        break;
                    }
                }
            }
            System.out.print(found + " ");
        }
        System.out.println();
    }

    void read() {
        Scanner sc = new Scanner(System.in);
        nFacility = sc.nextInt();
        nConsumer = sc.nextInt();

        setupCost = new double[nFacility];
        allSetupCost = new double[nFacility];
        capacity = new double[nFacility];
        allCapacity = new double[nFacility];
        demand = new double[nConsumer];
        x_facility = new double[nFacility];
        y_facility = new double[nFacility];
        x_consumer = new double[nConsumer];
        y_consumer = new double[nConsumer];
        transport = new double[nFacility][nConsumer];

        for(int i = 0; i < nFacility; ++i) {
            setupCost[i] = sc.nextDouble();
            capacity[i] = sc.nextDouble();
            x_facility[i] = sc.nextDouble();
            y_facility[i] = sc.nextDouble();

            allCapacity[i] = capacity[i];
            allSetupCost[i] = setupCost[i];
        }

        Arrays.sort(allCapacity);
        capacityLimit = allCapacity[nFacility - NFACILITY];
        capacityLimit = 0;
        Arrays.sort(allSetupCost);
        setupCostLimit = allSetupCost[nFacility - NFACILITY];
        // setupCostLimit = 50;

        for(int i = 0; i < nConsumer; ++i) {
            demand[i] = sc.nextDouble();
            x_consumer[i] = sc.nextDouble();
            y_consumer[i] = sc.nextDouble();
        }

        for(int i = 0; i < nFacility; ++i)
            for(int j = 0; j < nConsumer; ++j) {
                transport[i][j] = Math.sqrt(sqr(x_facility[i] - x_consumer[j])
                        + sqr(y_facility[i] - y_consumer[j]));
            }
    }

    double sqr(double x) {
        return x*x;
    }
}
