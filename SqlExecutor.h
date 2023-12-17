#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QVariantMap>
#include <qdatetime.h>
#include <qdebug.h>
class SqlExecutor {
public:
    //explicit SqlExecutor(const QString &connectionString) {
    //    db = QSqlDatabase::addDatabase("QODBC");
    //    db.setDatabaseName(connectionString);
    //}
    SqlExecutor(const QString& sqlLitepath) {
        if (QSqlDatabase::contains("my_unique_connection")) {
            db = QSqlDatabase::database("my_unique_connection");
        }
        else {
            db = QSqlDatabase::addDatabase("QSQLITE", "my_unique_connection");
            db.setDatabaseName(sqlLitepath);
        }
    }
    ~SqlExecutor() {
        db.close();
		//delete &db;
    }
    /**
	 * @brief 查询第一行第一列，如果没有就返回T的默认值
	 * @tparam T 要返回的类型
     * @param query 要执行的语句 
	 * @param nullValue 为空时返回的值
     * @return 
    */
    template<typename T>
    T executeScalar(const QString &query, T nullValue = T()) {
        if (!db.open()) {
            throw std::runtime_error(db.lastError().text().toStdString());
        }

        QSqlQuery q(db);

        if (!q.exec(query)) {
            throw std::runtime_error(q.lastError().text().toStdString());
        }
        if (!q.next())
        {
            db.close();
			return nullValue;
        }
		QVariant v = q.value(0);
		if (v.isNull())
		{
            db.close();
			return nullValue;
		}
        db.close();
		return v.value<T>();
    }
	/**
	 * @brief 查询第一列的值
	 * @tparam T 要返回的类型
	 * @param query 要执行的语句
	 * @param nullValue 为空时返回的值
	 * @return 
	*/
	template<typename T>
	QList<T> executeFirstColumn(const QString& query,T nullValue = T()) {
		if (!db.open()) {
			throw std::runtime_error(db.lastError().text().toStdString());
		}
		QSqlQuery q(db);
		if (!q.exec(query)) {
			throw std::runtime_error(q.lastError().text().toStdString());
		}
		QList<T> list;
		while (q.next()) {
			QVariant v = q.value(0);
            if (v.isNull())
            {
				list.append(nullValue);
			}
            else
            {
                list.append(v.value<T>());
            }
		}
        db.close();
		return list;
	}
    /**
	 * @brief 查询第一行
	 * @param query 要执行的语句
	 * @return 带有字段名的QMap
    */
    QMap<QString, QVariant> executeFirstRow(const QString& query) {
		if (!db.open()) {
			throw std::runtime_error(db.lastError().text().toStdString());
		}
		QSqlQuery q(db);
		if (!q.exec(query)) {
			throw std::runtime_error(q.lastError().text().toStdString());
		}
        else
        {
			QMap<QString, QVariant> map;
            if(q.next())
			{
                QSqlRecord rec = q.record();
                for (int i = 0; i < rec.count(); i++) {
                    map.insert(rec.fieldName(i), q.value(i));
                }
			}
			db.close();
			return map;
        }
    }
    /**
	 * @brief 执行非查询语句
	 * @param query 要执行的语句
	 * @param useTransaction 是否开启事务
	 * @return 受影响的行数
    */
    int executeNonQuery(const QString &query, bool useTransaction = true) {
        if (!db.open()) {
            throw std::runtime_error(db.lastError().text().toStdString());
        }

        QSqlQuery q(db);

        if (useTransaction && !db.transaction()) {
            throw std::runtime_error(db.lastError().text().toStdString());
        }
        try
        {
            if (!q.exec(query)) {
                if (useTransaction) {
                    db.rollback();
                }
                throw std::runtime_error(q.lastError().text().toStdString());
            }
        }
        catch (...)
        {
        }


        if (useTransaction && !db.commit()) {
            throw std::runtime_error(db.lastError().text().toStdString());
        }
		int r = q.numRowsAffected();
		db.close();
        return r;
    }
    /**
	 * @brief 执行查询
	 * @param query 要执行的语句
	 * @param useTransaction 是否开启事务
	 * @return 返回一个QVariantMap列表
    */
    QList<QVariantMap> executeQuery(const QString &query, bool useTransaction = true) {
        if (!db.open()) {
            throw std::runtime_error(db.lastError().text().toStdString());
        }

        QSqlQuery q(db);

        if (useTransaction && !db.transaction()) {
            throw std::runtime_error(db.lastError().text().toStdString());
        }

        if (!q.exec(query)) {
            if (useTransaction) {
                db.rollback();
            }
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        if (useTransaction && !db.commit()) {
            throw std::runtime_error(db.lastError().text().toStdString());
        }

        QList<QVariantMap> result;
        while (q.next()) {
            QVariantMap row;
            QSqlRecord rec = q.record();
            for(int i=0; i<rec.count(); i++) {
                row.insert(rec.fieldName(i), q.value(i));
            }
            result.append(row);
        }
		db.close();
        return result;
    }

private:
    QSqlDatabase db;
};