#pragma once

#include <QStyledItemDelegate>

class NoFocusDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	NoFocusDelegate();
	~NoFocusDelegate();

	void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const;
};
