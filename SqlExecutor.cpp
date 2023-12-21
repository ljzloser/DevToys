//#include "SqlExecutor.h"
//
//SqlExecutor::SqlExecutor(const QString& sqlLitepath)
//{
//	if (QSqlDatabase::contains(sqlLitepath)) {
//		db = QSqlDatabase::database(sqlLitepath);
//	}
//	else {
//		db = QSqlDatabase::addDatabase("QSQLITE", sqlLitepath);
//		db.setDatabaseName(sqlLitepath);
//	}
//}
//QVariant SqlExecutor::executeScalar(const QString& query)
//{
//	if (!db.open()) {
//		throw std::runtime_error(db.lastError().text().toStdString());
//	}
//	QSqlQuery q(db);
//	if (!q.exec(query)) {
//		throw std::runtime_error(q.lastError().text().toStdString());
//	}
//	if (!q.next())
//	{
//		return QVariant();
//	}
//	QVariant v = q.value(0);
//	if (v.isNull())
//	{
//		return QVariant();
//	}
//	return v;
//}
//QVariantList SqlExecutor::executeFirstColumn(const QString& query)
//{
//	if (!db.open()) {
//		throw std::runtime_error(db.lastError().text().toStdString());
//	}
//	QSqlQuery q(db);
//	if (!q.exec(query)) {
//		throw std::runtime_error(q.lastError().text().toStdString());
//	}
//	QVariantList list;
//	while (q.next()) {
//		QVariant v = q.value(0);
//		list.append(v);
//	}
//	return list;
//}
//QVariantMap SqlExecutor::executeFirstRow(const QString& query)
//{
//	QVariantMap map;
//	QSqlRecord rec = executeFirstRecord(query);
//	for (int i = 0; i < rec.count(); i++) {
//		map.insert(rec.fieldName(i), rec.value(i));
//	}
//	return map;
//}
//QSqlRecord SqlExecutor::executeFirstRecord(const QString& query)
//{
//	if (!db.open()) {
//		throw std::runtime_error(db.lastError().text().toStdString());
//	}
//	QSqlQuery q(db);
//	if (!q.exec(query)) {
//		throw std::runtime_error(q.lastError().text().toStdString());
//	}
//	else
//	{
//		QSqlRecord rec;
//		if (q.next())
//		{
//			rec = q.record();
//		}
//		return rec;
//	}
//}
//int SqlExecutor::executeNonQuery(const QString& query, bool useTransaction)
//{
//	if (!db.open()) {
//		throw std::runtime_error(db.lastError().text().toStdString());
//	}
//
//	QSqlQuery q(db);
//
//	if (useTransaction && !db.transaction()) {
//		throw std::runtime_error(db.lastError().text().toStdString());
//	}
//
//	if (!q.exec(query)) {
//		if (useTransaction) {
//			db.rollback();
//		}
//		throw std::runtime_error(q.lastError().text().toStdString());
//	}
//	if (useTransaction && !db.commit()) {
//		throw std::runtime_error(db.lastError().text().toStdString());
//	}
//	int r = q.numRowsAffected();
//
//	return r;
//}
//QList<QVariantMap> SqlExecutor::executeQuery(const QString& query)
//{
//	QList<QSqlRecord> list = executeQueryRecord(query);
//	QList<QVariantMap> result;
//	for (QSqlRecord r : list)
//	{
//		QVariantMap map;
//		for (int i = 0; i < r.count(); i++) {
//			map.insert(r.fieldName(i), r.value(i));
//		}
//		result.append(map);
//	}
//	return result;
//}
//QList<QSqlRecord> SqlExecutor::executeQueryRecord(const QString& query)
//{
//	if (!db.open()) {
//		throw std::runtime_error(db.lastError().text().toStdString());
//	}
//
//	QSqlQuery q(db);
//
//	if (!q.exec(query)) {
//		throw std::runtime_error(q.lastError().text().toStdString());
//	}
//
//	QList<QSqlRecord> result;
//	while (q.next())
//	{
//		result.append(q.record());
//	}
//	return result;
//}
//QStandardItemModel* SqlExecutor::executeQueryModel(const QString& query)
//{
//	QList<QSqlRecord> list = executeQueryRecord(query);
//	QStandardItemModel* model = new QStandardItemModel();
//	for (int i = 0; i < list.count(); i++)
//	{
//		if (i == 0)
//		{
//			for (int j = 0; j < list.at(i).count(); j++)
//			{
//				model->setHorizontalHeaderItem(j, new QStandardItem(list.at(i).fieldName(j)));
//			}
//		}
//		for (int j = 0; j < list.at(i).count(); j++)
//		{
//			model->setItem(i, j, new QStandardItem(list.at(i).value(j).toString()));
//		}
//	}
//	return model;
//}
//template<typename T>
//inline T SqlExecutor::executeScalar(const QString& query, T nullValue)
//{
//	{
//		QVariant v = executeScalar(query);
//		return v.isNull() ? nullValue : v.value<T>();
//	}
//}
//
//template<typename T>
//QList<T> SqlExecutor::executeFirstColumn(const QString& query, T nullValue)
//{
//	QVariantList list = executeFirstColumn(query);
//	QList<T> result;
//	for (QVariant v : list)
//	{
//		result.append(v.isNull() ? nullValue : v.value<T>());
//	}
//	return result;
//}