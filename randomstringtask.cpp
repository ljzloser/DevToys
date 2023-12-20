#include "randomstringtask.h"

RandomStringTask::RandomStringTask(int length, QString* result, QMutex* mutex, int offset, QAtomicInt* atomicInt, QList<int> type)
	: QRunnable()
{
	this->m_length = length;
	this->m_result = result;
	this->m_mutex = mutex;
	this->m_offset = offset;
	this->m_type = type;
	this->m_atomicInt = atomicInt;
}

RandomStringTask::~RandomStringTask()
{}

void RandomStringTask::run()
{
	QString result;
	result.reserve(m_length);
	int typeCount = m_type.count();
	for (int i = 0; i < m_length; i++)
	{
		int key = QRandomGenerator::global()->bounded(typeCount);
		int index = QRandomGenerator::global()->bounded(randomStringCharMap.at(key).length());
		result.append(randomStringCharMap.at(key).at(index));
		this->m_atomicInt->fetchAndAddRelaxed(1);
		if (i % 100 == 0)
		{
			emit this->progress(this->m_atomicInt->loadRelaxed());
		}
	}

	m_mutex->lock();
	m_result->replace(m_offset, m_length, result);
	m_mutex->unlock();
	emit this->progress(this->m_atomicInt->loadRelaxed());
}