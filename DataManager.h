#pragma once

#include <QtCore/QString>
#include <osg/Node>
#include <osg/MatrixTransform>

class DataManager
{
public:

	enum PathType
	{
		PathType_Circle = 0,
		PathType_Line = 1
	};

	static DataManager* Instance();

	void LoadAerocraft(const QString& strFile);

	void SetAerocraftRotate(float x, float y, float z);

	void GetAerocraftRotate(float& x, float& y, float& z);

	void SetAerocraftScale(double scale);

	double GetAerocraftScale();

	void LoadTargetObject(const QString& strFile);

	osg::Node* GetAerocraftNode();

	osg::Node* GetObservedObjectNode();

	osg::Node* GetTerrainNode();

	osg::Node* GetRootNode();

	void LoadTerrain();

	void SetCirclePathPara(const osg::Vec3d& vecCenter, double dRadius, double dTime);

	void GetCirclePathPara(osg::Vec3d& vecCenter, double& dRadius, double& dTime);

	void SetLinePathPara(const osg::Vec3d& vecStartPoint, const osg::Vec3d& vecEndPoint, double dTime);

	void GetLinePathPara(osg::Vec3d& startPoint, osg::Vec3d& endPoint, double& dTime);

	void SetPathType(PathType type);

	void GetTargetPara(double& dLon, double& dLat, double& dHeight, double& dRotateX
		, double& dRotateY, double& dRotateZ, double& dScale);

	void SetTargetPara(double dLon, double dLat, double dHeight, double dRotateX
		, double dRotateY, double dRotateZ, double dScale);

protected:

	osg::Group* m_pRoot;

	osg::Node* m_pAerocraftNode;

	osg::Node* m_pTerrainNode;

	osg::MatrixTransform* m_pAerocraftLocalMatrixNode;
	osg::PositionAttitudeTransform* m_pAerocraftAnimationNode;

	//�������ֲ���ת
	float m_dRotateX;
	float m_dRotateY;
	float m_dRotateZ;

	//���������ű���
	double m_dScale;

	//����·������
	osg::Vec3d m_circleCenter;
	//����·���뾶
	double m_dCircleRadius;
	//����·��ʱ��
	double m_dCircleTime;

	//ֱ��·��ʱ��
	double m_dLineTime;
	//ֱ��·����ʼ��
	osg::Vec3d m_lineStartPoint;
	//ֱ��·���յ�
	osg::Vec3d m_lineEndPoint;


	//Ŀ��ڵ�
	osg::Node* m_pTargetNode;

	osg::Vec3d m_vTargetPos;

	double m_dTargetRotateX;

	double m_dTargetRotateY;

	double m_dTargetRotateZ;

	//Ŀ��ֲ���ת����
	osg::MatrixTransform* m_pTargetLocalMatrixNode;
	osg::PositionAttitudeTransform* m_pTargetAnimationNode;
	//Ŀ�����ű���
	double m_dTargetScale;

	DataManager();
	virtual ~DataManager();

	DataManager& operator = (const DataManager&);
	DataManager(const DataManager&);
};
