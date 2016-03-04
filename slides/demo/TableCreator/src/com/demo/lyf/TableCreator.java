package com.demo.lyf;

import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by yifengliu on 16/2/29.
 */
public class TableCreator {
    //实现创建表
    public static void main(String[] args) {
        TableCreator tc = new TableCreator();
        tc.executeCreateDB(Member.class);
    }

    public Connection getConnection() {
        try {
            Class.forName("org.sqlite.JDBC");
            Connection con = DriverManager.getConnection("jdbc:sqlite:test.db");
            return con;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public void executeCreateDB(Class<?> entity) {
        String sqlStr = explainAnnotation(entity);
        Connection con = getConnection();
        PreparedStatement psql = null;
        if (con != null && !sqlStr.equals("error")) {
            try {
                psql = con.prepareStatement(sqlStr);
                psql.execute();
            } catch (SQLException e) {
                e.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                try {
                    if (psql != null)
                        psql.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
                try {
                    if (con != null)
                        psql.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            }
        } else
            System.out.println("failure to...");
    }

    // 真正的处理器,Class<?>必须用这个表明
    public String explainAnnotation(Class<?> entity) {
        // 获取指定类型的注解
        DBTable dbtable = entity.getAnnotation(DBTable.class);
        if (dbtable == null) {
            System.out.println("No DBTable annotation in class"
                    + entity.getName());
            return "error";
        } else {
            String tableName = dbtable.name();// 获取注解name值，即表名称
            // 当没有设置name值，直接利用类的名作为表名
            if (tableName.length() < 1)
                tableName = entity.getName().toUpperCase();// 转换大写
            // 准备处理字段注解
            List<String> columnsDefs = new ArrayList<String>();
            // 获取该类的所有字段
            for (Field field : entity.getDeclaredFields()) {
                String columnName = null;
                // 获取该字段所有的注解
                Annotation[] anns = field.getDeclaredAnnotations();
                // Annotation[] anns=field.getAnnotations();
                // 当有注解的时候
                if (anns.length >= 1) {
                    // 判断注解的类型
                    if (anns[0] instanceof SQLInteger) {
                        SQLInteger sInt = (SQLInteger) anns[0];
                        // 当没有name时候，将字段大写为列名
                        if (sInt.name().length() < 1)
                            columnName = field.getName().toUpperCase();
                        else
                            columnName = sInt.name();
                        columnsDefs.add(columnName + " INT"
                                + getConstraints(sInt.constraints()));
                    }
                    if (anns[0] instanceof SQLString) {
                        SQLString sString = (SQLString) anns[0];
                        // 当没有name时候，将字段大写为列名
                        if (sString.name().length() < 1)
                            columnName = field.getName().toUpperCase();
                        else
                            columnName = sString.name();
                        columnsDefs.add(columnName + " VARCHAR("
                                + sString.value() + ")"
                                + getConstraints(sString.constraints()));
                    }
                }
            }
            StringBuilder createDB = new StringBuilder("CREATE TABLE "
                    + tableName + "(");
            for (String cols : columnsDefs)
                createDB.append(" " + cols + ",");
            // 移除最后一个，号
            String tableSQL = createDB.substring(0, createDB.length() - 1)
                    + ");";
            // 输出创建表的过程
            System.out.println("Table Creation SQL is:\n" + tableSQL);
            return tableSQL;
        }
    }

    // 返回指定的约束
    public String getConstraints(Constraints con) {
        String constras = "";
        if (!con.allowNull())
            constras += " NOT NULL";
        if (con.primaryKey())
            constras += " PRIMARY KEY";
        if (con.unique())
            constras += " UNIQUE";
        return constras;
    }
}
