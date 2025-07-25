#include "cpp_qt-gui/utils.h"
#include <QApplication>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QTreeWidget>
#include <QVBoxLayout>

class NetworkInteractionSettings : public QMainWindow {
	Q_OBJECT

  public:
	NetworkInteractionSettings(QWidget *parent = nullptr)
		: QMainWindow(parent) {
		QWidget *central = new QWidget(this);
		setCentralWidget(central);

		auto *mainLayout = new QHBoxLayout(central);

		tree = new QTreeWidget;
		tree->setHeaderLabel("Настройка сетевого взаимодействия");
		mainLayout->addWidget(tree, 1);

		QTreeWidgetItem *serversItem = new QTreeWidgetItem(tree);
		serversItem->setText(0, "Серверы");
		serversItem->setFlags(serversItem->flags() & ~Qt::ItemIsSelectable);

		stations = parseConfig();
		Station new_station = Station();
		addStation(serversItem, new_station);
		for (const auto &station : stations) {
			addStation(serversItem, station);
		}

		tree->expandItem(serversItem);

		auto *rightWidget = new QWidget;
		auto *rightLayout = new QVBoxLayout(rightWidget);

		formLayout = new QFormLayout;

		stationNameEdit = new QLineEdit;
		formLayout->addRow("Название", stationNameEdit);

		stationPortSpin = new QSpinBox;
		stationPortSpin->setRange(1, 65535);
		formLayout->addRow("Порт", stationPortSpin);

		stationTimeoutSpin = new QSpinBox;
		stationTimeoutSpin->setRange(0, 1000);
		formLayout->addRow("Тайм-аут T0, сек", stationTimeoutSpin);

		rightLayout->addLayout(formLayout);

		saveButton = new QPushButton("Сохранить");
		rightLayout->addWidget(saveButton);

		rightLayout->addStretch(1);
		mainLayout->addWidget(rightWidget, 2);

		connect(tree, &QTreeWidget::currentItemChanged, this,
				&NetworkInteractionSettings::onTreeItemSelected);
		connect(saveButton, &QPushButton::clicked, this,
				&NetworkInteractionSettings::onSaveClicked);
		connect(stationNameEdit, &QLineEdit::textChanged, this,
				&NetworkInteractionSettings::onStationNameChanged);
		connect(stationNameEdit, &QLineEdit::editingFinished, this,
				&NetworkInteractionSettings::onStationNameEditingFinished);

		if (serversItem->childCount() > 0) {
			tree->setCurrentItem(serversItem->child(0));
		}
	}

  private slots:
	void onTreeItemSelected(QTreeWidgetItem *current, QTreeWidgetItem *) {
		if (!current)
			return;
		if (current->parent() && current->parent()->text(0) == "Серверы") {
			// stations setting
			stationNameEdit->setText(current->text(0));
			stationPortSpin->setValue(
				current->data(0, Qt::UserRole + 1).toInt());
			stationTimeoutSpin->setValue(
				current->data(0, Qt::UserRole + 2).toInt());

			formLayout->setEnabled(true);
			saveButton->setEnabled(true);
			currentItem = current;
		} else if (current->text(0) == "Серверы") {
			formLayout->setEnabled(false);
			saveButton->setEnabled(false);
			currentItem = nullptr;
		}
	}

	void onSaveClicked() {
		if (!currentItem)
			return;
		currentItem->setText(0, stationNameEdit->text());
		currentItem->setData(0, Qt::UserRole + 1, stationPortSpin->value());
		currentItem->setData(0, Qt::UserRole + 2, stationTimeoutSpin->value());

		bool containsStation = std::any_of(
			stations.begin(), stations.end(), [&](const Station &s) {
				return QString::fromStdString(s.name) ==
					   stationNameEdit->text();
			});

		if (!containsStation) {
			Station station = Station();
			station.name = stationNameEdit->text().toStdString();
			station.port = stationPortSpin->value();
			station.timeout1 = stationTimeoutSpin->value();
			stations.push_back(station);
		}

		writeConfig(stations);
	}

	void onStationNameChanged(const QString &text) {
		if (!validateStationName(text)) {
			stationNameEdit->setStyleSheet(
				QString::fromUtf8("background-color: rgb(255, 200, 200); "
								  "border: 1px solid red;"));
		} else {
			stationNameEdit->setStyleSheet(QString());
		}
	}

	void onStationNameEditingFinished() {
		QString trimmed = stationNameEdit->text().trimmed();
		if (stationNameEdit->text() != trimmed) {
			stationNameEdit->setText(trimmed);
		}
	}

  private:
	void addStation(QTreeWidgetItem *serversItem, const Station &station) {
		QTreeWidgetItem *stationItem = new QTreeWidgetItem(serversItem);
		stationItem->setText(0, QString::fromStdString(station.name));
		stationItem->setData(0, Qt::UserRole + 1, station.port);
		stationItem->setData(0, Qt::UserRole + 2, station.timeout1);
	}

	QTreeWidget *tree;
	QFormLayout *formLayout;
	QLineEdit *stationNameEdit;
	QSpinBox *stationPortSpin;
	QSpinBox *stationTimeoutSpin;
	QPushButton *saveButton;
	QTreeWidgetItem *currentItem = nullptr;
	std::vector<Station> stations;
};

#include "main.moc"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	NetworkInteractionSettings window;
	window.resize(600, 400);
	window.setWindowTitle("Настройка сетевого взаимодействия");
	window.show();
	return app.exec();
}
