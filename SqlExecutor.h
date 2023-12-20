#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QVariantMap>
#include <qdatetime.h>
#include <qdebug.h>
#include <qstandarditemmodel.h>

class SqlExecutor {
public:
	//explicit SqlExecutor(const QString &connectionString) {
	//    db = QSqlDatabase::addDatabase("QODBC");
	//    db.setDatabaseName(connectionString);
	//}
	SqlExecutor(const QString& sqlLitepath) {
		if (QSqlDatabase::contains(sqlLitepath)) {
			db = QSqlDatabase::database(sqlLitepath);
		}
		else {
			db = QSqlDatabase::addDatabase("QSQLITE", sqlLitepath);
			db.setDatabaseName(sqlLitepath);
		}
	}
	~SqlExecutor() {

	}
	/**
	 * @brief 查询第一行第一列，如果没有就返回T的默认值
	 * @tparam T 要返回的类型
	 * @param query 要执行的语句
	 * @param nullValue 为空时返回的值
	 * @return 返回T
	*/
	template<typename T>
	T executeScalar(const QString& query, T nullValue = T()) {
		QVariant v = executeScalar(query);
		return v.isNull() ? nullValue : v.value<T>();
	}
	/**
	 * @brief 查询第一行第一列
	 * @param query 要执行的语句
	 * @return 返回QVariant
	*/
	QVariant executeScalar(const QString& query) {
		if (!db.open()) {
			throw std::runtime_error(db.lastError().text().toStdString());
		}
		QSqlQuery q(db);
		if (!q.exec(query)) {
			throw std::runtime_error(q.lastError().text().toStdString());
		}
		if (!q.next())
		{
			return QVariant();
		}
		QVariant v = q.value(0);
		if (v.isNull())
		{
			return QVariant();
		}
		return v;
	}
	/**
	 * @brief 查询第一列的值
	 * @tparam T 要返回的类型
	 * @param query 要执行的语句
	 * @param nullValue 为空时返回的值
	 * @return
	*/
	template<typename T>
	QList<T> executeFirstColumn(const QString& query, T nullValue = T()) {
		QVariantList list = executeFirstColumn(query);
		QList<T> result;
		for (QVariant v : list)
		{
			result.append(v.isNull() ? nullValue : v.value<T>());
		}
		return result;
	}
	/**
	 * @brief 查询第一列
	 * @param query 要执行的语句
	 * @return 反回QVariantList
	*/
	QVariantList executeFirstColumn(const QString& query) {
		if (!db.open()) {
			throw std::runtime_error(db.lastError().text().toStdString());
		}
		QSqlQuery q(db);
		if (!q.exec(query)) {
			throw std::runtime_error(q.lastError().text().toStdString());
		}
		QVariantList list;
		while (q.next()) {
			QVariant v = q.value(0);
			list.append(v);
		}
		return list;
	}
	/**
	 * @brief 查询第一行
	 * @param query 要执行的语句
	 * @return QVariantMap
	*/
	QVariantMap executeFirstRow(const QString& query) {
		QVariantMap map;
		QSqlRecord rec = executeFirstRecord(query);
		for (int i = 0; i < rec.count(); i++) {
			map.insert(rec.fieldName(i), rec.value(i));
		}
		return map;
	}
	/**
	 * @brief 查询第一行
	 * @param query 要执行的语句
	 * @return QSqlRecord
	*/
	QSqlRecord executeFirstRecord(const QString& query) {
		if (!db.open()) {
			throw std::runtime_error(db.lastError().text().toStdString());
		}
		QSqlQuery q(db);
		if (!q.exec(query)) {
			throw std::runtime_error(q.lastError().text().toStdString());
		}
		else
		{
			QSqlRecord rec;
			if (q.next())
			{
				rec = q.record();
			}
			return rec;
		}
	}
	/**
	 * @brief 执行非查询语句
	 * @param query 要执行的语句
	 * @param useTransaction 是否开启事务
	 * @return 受影响的行数
	*/
	int executeNonQuery(const QString& query, bool useTransaction = true) {
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

		return r;
	}
	/**
	 * @brief 执行查询
	 * @param query 要执行的语句
	 * @param useTransaction 是否开启事务
	 * @return 返回一个QVariantMap列表
	*/
	QList<QVariantMap> executeQuery(const QString& query)
	{
		QList<QSqlRecord> list = executeQueryRecord(query);
		QList<QVariantMap> result;
		for (QSqlRecord r : list)
		{
			QVariantMap map;
			for (int i = 0; i < r.count(); i++) {
				map.insert(r.fieldName(i), r.value(i));
			}
			result.append(map);
		}
		return result;
	}
	/**
	 * @brief 执行查询
	 * @param query 要执行的语句
	 * @param useTransaction 是否开启事务
	 * @return 返回一个QSqlRecord列表
	*/
	QList<QSqlRecord> executeQueryRecord(const QString& query)
	{
		if (!db.open()) {
			throw std::runtime_error(db.lastError().text().toStdString());
		}

		QSqlQuery q(db);

		if (!q.exec(query)) {
			throw std::runtime_error(q.lastError().text().toStdString());
		}

		QList<QSqlRecord> result;
		while (q.next())
		{
			result.append(q.record());
		}
		return result;
	}
	/**
	 * @brief 执行查询，返回一个QStandardItemModel
	 * @param query 要执行的语句
	 * @return 返回一个QStandardItemModel
	*/
	QStandardItemModel* executeQueryModel(const QString& query)
	{
		QList<QSqlRecord> list = executeQueryRecord(query);
		QStandardItemModel* model = new QStandardItemModel();
		for (int i = 0; i < list.count(); i++)
		{
			if (i == 0)
			{
				for (int j = 0; j < list.at(i).count(); j++)
				{
					model->setHorizontalHeaderItem(j, new QStandardItem(list.at(i).fieldName(j)));
				}
			}
			for (int j = 0; j < list.at(i).count(); j++)
			{
				model->setItem(i, j, new QStandardItem(list.at(i).value(j).toString()));
			}
		}
		return model;
	}

private:
	QSqlDatabase db;
};