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
	 * @brief ��ѯ��һ�е�һ�У����û�оͷ���T��Ĭ��ֵ
	 * @tparam T Ҫ���ص�����
     * @param query Ҫִ�е���� 
	 * @param nullValue Ϊ��ʱ���ص�ֵ
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
	 * @brief ��ѯ��һ�е�ֵ
	 * @tparam T Ҫ���ص�����
	 * @param query Ҫִ�е����
	 * @param nullValue Ϊ��ʱ���ص�ֵ
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
	 * @brief ��ѯ��һ��
	 * @param query Ҫִ�е����
	 * @return �����ֶ�����QMap
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
	 * @brief ִ�зǲ�ѯ���
	 * @param query Ҫִ�е����
	 * @param useTransaction �Ƿ�������
	 * @return ��Ӱ�������
    */
    int executeNonQuery(const QString &query, bool useTransaction = true) {
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
		int r = q.numRowsAffected();
		db.close();
        return r;
    }
    /**
	 * @brief ִ�в�ѯ
	 * @param query Ҫִ�е����
	 * @param useTransaction �Ƿ�������
	 * @return ����һ��QVariantMap�б�
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