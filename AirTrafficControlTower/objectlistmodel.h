#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractListModel>

#include "RadarDataReceiver/object.h"

class ObjectListModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum ObjectRoles
	{
		IdRole = Qt::UserRole + 1,
		LatitudeRole,
		LongitudeRole,
		IconRole,
		AzimuthRole
	};

	explicit ObjectListModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

	void addObject(Object *obj);
	Object *getObject(int index) const;
	void updateObject(int index, Object *obj);
	Q_INVOKABLE void removeObject(int row);

private:
	QList<Object *> m_objects;
};

#endif // OBJECTLISTMODEL_H
