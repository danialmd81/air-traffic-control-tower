#include "objectlistmodel.h"

ObjectListModel::ObjectListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int ObjectListModel::rowCount(const QModelIndex &) const
{
	return m_objects.size();
}

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() >= m_objects.size())
		return QVariant();

	Object *obj = m_objects.at(index.row());

	switch (role)
	{
		case IdRole:
			return obj->getId();
		case LatitudeRole:
			return obj->getLatitude();
		case LongitudeRole:
			return obj->getLongitude();
		case IconRole:
			if (obj->getType() == "airplane") return "qrc:/Image/airplane.png";
			if (obj->getType() == "jet") return "qrc:/Image/jet.png";
			if (obj->getType() == "helicopter") return "qrc:/Image/helicopter.png";
			return "qrc:/Image/airplane.png";
	}
	return QVariant();
}

QHash<int, QByteArray> ObjectListModel::roleNames() const
{
	return {
		{ IdRole, "id" },
		{ LatitudeRole, "latitude" },
		{ LongitudeRole, "longitude" },
		{ IconRole, "icon" }
	};
}

void ObjectListModel::addObject(Object *obj)
{
	beginInsertRows(QModelIndex(), m_objects.size(), m_objects.size());
	m_objects.append(obj);
	endInsertRows();
}