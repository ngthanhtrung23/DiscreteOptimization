// Compile: javac -classpath "localsolver.jar:." Solution.java
// Run:     java -classpath "localsolver.jar:." Solution
import localsolver.*;

import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class Solution
{
    public static final String INPUT_FILENAME = "LocalSearch/input/tsp_574_1";
    public static final String BACKUP_OUTPUT = "output_4.txt";
    public static void main(String[] args) {
        (new Solver2()).go();
    }
}

class Solver
{
    int n;
    double[] x_pos;
    double[] y_pos;

    public void go() {
        readInput();
        solve();
    }

    void solve() {
        try {
            // Create model
            LocalSolver localsolver = new LocalSolver();
            LSModel model = localsolver.getModel();

            // Add variables: x[i][j] = true <=> j-th city is visited at i-th turn
            LSExpression[][] x = new LSExpression[n][n];
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < n; ++j) {
                    x[i][j] = model.createExpression(LSOperator.Bool);
                    x[i][j].setName("x[" + i + "," + j + "]");
                }

            // Constraints
            for(int i = 0; i < n; ++i) {
                LSExpression sumRow = model.createExpression(LSOperator.Sum);
                for(int j = 0; j < n; ++j) {
                    sumRow.addOperand(x[i][j]);
                }
                model.addConstraint(model.createExpression(LSOperator.Eq, sumRow, 1));
            }
            for(int j = 0; j < n; ++j) {
                LSExpression sumCol = model.createExpression(LSOperator.Sum);
                for(int i = 0; i < n; ++i) {
                    sumCol.addOperand(x[i][j]);
                }
                model.addConstraint(model.createExpression(LSOperator.Eq, sumCol, 1));
            }

            // Objective function
            LSExpression obj = model.createExpression(LSOperator.Sum);
            for(int from = 0; from < n; ++from) {
                int to = (from == n-1) ? 0 : (from + 1);
                for(int u = 0; u < n; ++u)
                    for(int v = 0; v < n; ++v) {
                        LSExpression prod = model.createExpression(LSOperator.Prod);
                        prod.addOperand(x[from][u]);
                        prod.addOperand(x[to][v]);
                        prod.addOperand(Math.sqrt(
                                sqr(x_pos[u] - x_pos[v])
                                + sqr(y_pos[u] - y_pos[v])
                        ));
                        obj.addOperand(prod);
                    }
            }
            model.addObjective(obj, LSObjectiveDirection.Minimize);
            model.close();

            LSPhase phase = localsolver.createPhase();
            phase.setTimeLimit(60);
            localsolver.solve();

            System.out.println(" 0");
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j)
                    if (x[i][j].getValue() == 1) {
                        System.out.print(j + " " );
                        break;
                    }
            }
            System.out.println();
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    void readInput() {
        try {
            Scanner sc = new Scanner(new File(Solution.INPUT_FILENAME));
            n = sc.nextInt();
            x_pos = new double[n];
            y_pos = new double[n];
            for(int i = 0; i < n; ++i) {
                x_pos[i] = sc.nextDouble();
                y_pos[i] = sc.nextDouble();
            }
        }
        catch (IOException ex) {
            System.out.println("Cannot read input file");
        }
    }

    double sqr(double x) {
        return x*x;
    }
}

class Solver2 extends Solver
{
    int[] path;

    @Override
    void solve() {
        readCurrentSolution();
        try {
            // Create model
            LocalSolver localsolver = new LocalSolver();
            LSModel model = localsolver.getModel();

            // Add variables: x[i][j] = true <=> j-th city is visited at i-th turn
            LSExpression[][] x = new LSExpression[n][n];
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < n; ++j) {
                    x[i][j] = model.createExpression(LSOperator.Bool);
                    x[i][j].setName("x[" + i + "," + j + "]");
                }

            // Constraints
            for(int i = 0; i < n; ++i) {
                LSExpression sumRow = model.createExpression(LSOperator.Sum);
                for(int j = 0; j < n; ++j) {
                    sumRow.addOperand(x[i][j]);
                }
                model.addConstraint(model.createExpression(LSOperator.Eq, sumRow, 1));
            }
            for(int j = 0; j < n; ++j) {
                LSExpression sumCol = model.createExpression(LSOperator.Sum);
                for(int i = 0; i < n; ++i) {
                    sumCol.addOperand(x[i][j]);
                }
                model.addConstraint(model.createExpression(LSOperator.Eq, sumCol, 1));
            }

            // Objective function
            LSExpression distance = model.createExpression(LSOperator.Array);
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < n; ++j)
                    distance.addOperand(
                            Math.sqrt(
                                    sqr(x_pos[i] - x_pos[j])
                                    + sqr(y_pos[i] - y_pos[j])
                            )
                    );

            LSExpression obj = model.createExpression(LSOperator.Sum);
            LSExpression[] city = new LSExpression[n];
            for(int i = 0; i < n; ++i) {
                city[i] = model.createExpression(LSOperator.Sum);
                for(int j = 0; j < n; ++j) {
                    LSExpression prod = model.createExpression(LSOperator.Prod);
                    prod.addOperand(j);
                    prod.addOperand(x[i][j]);
                    city[i].addOperand(prod);
                }
            }

            LSExpression[] distanceCode = new LSExpression[n];
            int maxCode = n*n-1;
            for(int i = 0; i < n; ++i) {
                int from = i, to = (i == n-1) ? 0 : (i+1);
                distanceCode[i] = model.createExpression(
                        LSOperator.Sum,
                        city[from],
                        model.createExpression(LSOperator.Prod, city[to], n)
                );
                obj.addOperand(model.createExpression(
                        LSOperator.At,
                        distance,
                        model.createExpression(LSOperator.Min, distanceCode[i], maxCode)
                ));
            }

            model.addObjective(obj, LSObjectiveDirection.Minimize);
            model.close();

            for(int i = 0; i < n; ++i)
                x[i][path[i]].setIntValue(1);

            LSPhase phase = localsolver.createPhase();
            phase.setTimeLimit(600);
            localsolver.solve();

            System.out.println(" 0");
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j)
                    if (x[i][j].getValue() == 1) {
                        System.out.print(j + " " );
                        break;
                    }
            }
            System.out.println();
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private void readCurrentSolution() {
        path = new int[n];
        try {
            Scanner sc = new Scanner(new File(Solution.BACKUP_OUTPUT));
            sc.nextDouble();
            sc.nextInt();
            for(int i = 0; i < n; ++i)
                path[i] = sc.nextInt();
        }
        catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
