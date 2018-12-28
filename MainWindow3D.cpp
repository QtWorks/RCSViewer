#include "MainWindow3D.h"
#include <osgViewer/Viewer>

#include "ViewerWidget.h"
#include <QtWidgets/QFileDialog>
#include "DataManager.h"
#include "AeroRotateDlg.h"
#include <QtWidgets/QActionGroup>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include "PathSettingsDlg.h"
#include "TargetSettingsDlg.h"
#include <QtWidgets/QDockWidget>
#include "RadarBeamRotateDlg.h"
#include "videoplayer.h"
#include <QtWidgets/QMenuBar>
#include "samplingthread.h"

extern osgViewer::View* g_pView;
extern SamplingThread* g_pSampleThread;

void SetTerrainManipulator();
void SetNodeTrackerManipulator(int nIndex = 0);

// class MyWidget : public QWidget
// {
// public:
// 	QSize sizeHint() const
// 	{
// 		return QSize(400, 900); /* �����ﶨ��dock�ĳ�ʼ��С */
// 	}
// };

MainWindow3D::MainWindow3D(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(QString::fromLocal8Bit("ģ��"));

	QMenu* pMenuFile = menuBar()->addMenu(QString::fromLocal8Bit("�ļ�"));
	QAction* pActionLoadAeroplane = pMenuFile->addAction(QString::fromLocal8Bit("���ط�����"));
	QAction* pActionLoadObservedObj = pMenuFile->addAction(QString::fromLocal8Bit("���ع۲�Ŀ��"));

	connect(pActionLoadAeroplane, SIGNAL(triggered()), this, SLOT(slotLoadAeroplane()));
	connect(pActionLoadObservedObj, SIGNAL(triggered()), this, SLOT(slotLoadObservedObj()));

	QMenu* pMenuManipulator = menuBar()->addMenu(QString::fromLocal8Bit("�ӽ�ģʽ"));
	QAction* pActionAero = pMenuManipulator->addAction(QString::fromLocal8Bit("������"));
	QAction* pActionTarget = pMenuManipulator->addAction(QString::fromLocal8Bit("Ŀ��"));
	QAction* pActionTerrain = pMenuManipulator->addAction(QString::fromLocal8Bit("����"));

	pActionAero->setCheckable(true);
	pActionTarget->setCheckable(true);
	pActionTerrain->setCheckable(true);

	pActionAero->setChecked(true);

	QActionGroup* pActonGroup = new QActionGroup(pMenuManipulator);
	pActonGroup->addAction(pActionAero);
	pActonGroup->addAction(pActionTarget);
	pActonGroup->addAction(pActionTerrain);

	connect(pActionAero, SIGNAL(triggered()), this, SLOT(slotSetManipulatorAeroplane()));
	connect(pActionTarget, SIGNAL(triggered()), this, SLOT(slotSetManipulatorTarget()));
	connect(pActionTerrain, SIGNAL(triggered()), this, SLOT(slotSetManipulatorTerrain()));

	QMenu* pMenuSettings = menuBar()->addMenu(QString::fromLocal8Bit("��������̬"));
	QAction* pActionAeroplanePos = pMenuSettings->addAction(QString::fromLocal8Bit("��������"));

	connect(pActionAeroplanePos, SIGNAL(triggered()), this, SLOT(slotSetAeroplaneMatrix()));

	QMenu* pMenuAeroplanePath = menuBar()->addMenu(QString::fromLocal8Bit("���й켣"));
	QAction* pCirclePath = pMenuAeroplanePath->addAction(QString::fromLocal8Bit("����"));
	QAction* pLinePath = pMenuAeroplanePath->addAction(QString::fromLocal8Bit("ֱ��"));
	pMenuAeroplanePath->addSeparator();
	QAction* pActionPathPara = pMenuAeroplanePath->addAction(QString::fromLocal8Bit("��������"));

	pCirclePath->setCheckable(true);
	pCirclePath->setChecked(true);
	pLinePath->setCheckable(true);

	QActionGroup* pActonGroup2 = new QActionGroup(pMenuAeroplanePath);
	pActonGroup2->addAction(pCirclePath);
	pActonGroup2->addAction(pLinePath);

	connect(pCirclePath, SIGNAL(triggered()), this, SLOT(slotCirclePath()));
	connect(pLinePath, SIGNAL(triggered()), this, SLOT(slotLinePath()));
	connect(pActionPathPara, SIGNAL(triggered()), this, SLOT(slotPathPara()));

	QMenu* pMenuTargetSettings = menuBar()->addMenu(QString::fromLocal8Bit("Ŀ�����"));
	QAction* pActionTargetSettings = pMenuTargetSettings->addAction(QString::fromLocal8Bit("����"));

	connect(pActionTargetSettings, SIGNAL(triggered()), this, SLOT(slotTargetPara()));

	QMenu* pMenuRadarBeamSettings = menuBar()->addMenu(QString::fromLocal8Bit("��������"));
	QAction* pActionRadarBeamSettings = pMenuRadarBeamSettings->addAction(QString::fromLocal8Bit("����"));

	connect(pActionRadarBeamSettings, SIGNAL(triggered()), this, SLOT(slotRadarBeamPara()));

#if QT_VERSION >= 0x050000
	// Qt5 is currently crashing and reporting "Cannot make QOpenGLContext current in a different thread" when the viewer is run multi-threaded, this is regression from Qt4
	osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::SingleThreaded;
#else
	osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
#endif

	ViewerWidget* viewWidget = new ViewerWidget(threadingModel);
	setCentralWidget(viewWidget);

	QDockWidget* pDockWidget = new QDockWidget;
 	VideoPlayer* pPlayer = new VideoPlayer;
 	pDockWidget->setWidget(pPlayer);
	
	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, pDockWidget);
	//pDockWidget->setFloating(true);

	m_pPlot = new Plot(this);

	g_pSampleThread->start();
	m_pPlot->start();
	//m_pPlot->setIntervalLength(10.0);

	QDockWidget* pDockWidget2 = new QDockWidget;
	pDockWidget2->setWidget(m_pPlot);
	addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, pDockWidget2);
	//pDockWidget2->setFloating(true);
}

MainWindow3D::~MainWindow3D()
{

}

void MainWindow3D::slotSetManipulatorAeroplane()
{
	QAction* pAction = qobject_cast<QAction*>(sender());
	pAction->setChecked(true);

	SetNodeTrackerManipulator();
}

void MainWindow3D::slotSetManipulatorTarget()
{
	QAction* pAction = qobject_cast<QAction*>(sender());
	pAction->setChecked(true);

	SetNodeTrackerManipulator(1);
}

void MainWindow3D::slotSetManipulatorTerrain()
{
	QAction* pAction = qobject_cast<QAction*>(sender());
	pAction->setChecked(true);

	SetTerrainManipulator();
}

void MainWindow3D::slotLoadAeroplane()
{
	QString strFileName = QFileDialog::getOpenFileName();
	if (strFileName.isEmpty())
		return;

	DataManager::Instance()->LoadAerocraft(strFileName);

	QString strClassName = g_pView->getCameraManipulator()->className();
	if (strClassName.compare("NodeTrackerManipulator") == 0)
	{
		SetNodeTrackerManipulator();
	}
}

void MainWindow3D::slotLoadObservedObj()
{
	QString strFileName = QFileDialog::getOpenFileName();
	if (strFileName.isEmpty())
		return;

	DataManager::Instance()->LoadTargetObject(strFileName);
}

void MainWindow3D::slotSetAeroplaneMatrix()
{
	AeroRotateDlg dlg;
	dlg.exec();
}

void MainWindow3D::slotCirclePath()
{
	QAction* pAction = qobject_cast<QAction*>(sender());
	pAction->setChecked(true);

	DataManager::Instance()->SetPathType(DataManager::PathType_Circle);
}

void MainWindow3D::slotLinePath()
{
	QAction* pAction = qobject_cast<QAction*>(sender());
	pAction->setChecked(true);

	DataManager::Instance()->SetPathType(DataManager::PathType_Line);
}

void MainWindow3D::slotPathPara()
{
	PathSettingsDlg dlg;
	dlg.exec();
}

void MainWindow3D::slotTargetPara()
{
	TargetSettingsDlg dlg;
	dlg.exec();
}

void MainWindow3D::slotRadarBeamPara()
{
	RadarBeamRotateDlg dlg;
	dlg.exec();
}