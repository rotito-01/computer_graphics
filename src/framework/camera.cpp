#include "camera.h"

#include "main/includes.h"
#include <iostream>

Camera::Camera()
{
	view_matrix.SetIdentity();
	SetOrthographic(-1,1,1,-1,-1,1);
}

Vector3 Camera::GetLocalVector(const Vector3& v)
{
	Matrix44 iV = view_matrix;
	if (iV.Inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.RotateVector(v);
	return result;
}

Vector3 Camera::ProjectVector(Vector3 pos)
{
	Vector4 pos4 = Vector4(pos.x, pos.y, pos.z, 1.0);
	Vector4 result = viewprojection_matrix * pos4;
	if (type == ORTHOGRAPHIC)
		return result.GetVector3();
	else
		return result.GetVector3() / result.w;
}

void Camera::Rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.MakeRotationMatrix(angle, axis);
	Vector3 new_front = R * (center - eye);
	center = eye + new_front;
	UpdateViewMatrix();
}

void Camera::Move(Vector3 delta)
{
	Vector3 localDelta = GetLocalVector(delta);
	eye = eye - localDelta;
	center = center - localDelta;
	UpdateViewMatrix();
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::SetPerspective(float fov, float aspect, float near_plane, float far_plane)
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	// Reset Matrix (Identity)
	view_matrix.SetIdentity();

	Vector3 forward = this->center - this->eye;
	Vector3 right = this->up.Cross(forward);
	Vector3 up1 = right.Cross(forward);

	// Comment this line to create your own projection matrix!
	//SetExampleViewMatrix();

	// Remember how to fill a Matrix4x4 (check framework slides)
	// Careful with the order of matrix multiplications, and be sure to use normalized vectors!
	
	// Create the view matrix rotation
	
	Matrix44 rotation = Matrix44();
	rotation.M[0][0] = right.x;
	rotation.M[0][1] = right.y;
	rotation.M[0][2] = right.z;
	rotation.M[1][0] = up1.x;
	rotation.M[1][1] = up1.y;
	rotation.M[1][2] = up1.z;
	rotation.M[2][0] = forward.x;
	rotation.M[2][1] = forward.y;
	rotation.M[2][2] = forward.z;

	// Translate view matrix
	Matrix44 translate = Matrix44();
	translate.M[0][3] = -this->eye.x;
	translate.M[1][3] = -this->eye.y;
	translate.M[2][3] = -this->eye.z;

	this->view_matrix = rotation * translate;

	UpdateViewProjectionMatrix();
}

// Create a projection matrix
void Camera::UpdateProjectionMatrix()
{
	// Reset Matrix (Identity)
	projection_matrix.SetIdentity();

	// Comment this line to create your own projection matrix!
	//SetExampleProjectionMatrix();

	// Remember how to fill a Matrix4x4 (check framework slides)
	
	if (type == PERSPECTIVE) {
		float f = (1 / tanf(this->fov / 2));
		projection_matrix.M[0][0] = f / this->aspect;
		projection_matrix.M[0][1] = 0;
		projection_matrix.M[0][2] = 0;
		projection_matrix.M[0][3] = 0;
		projection_matrix.M[1][0] = 0;
		projection_matrix.M[1][1] = f;
		projection_matrix.M[1][2] = 0;
		projection_matrix.M[1][3] = 0;
		projection_matrix.M[2][0] = 0;
		projection_matrix.M[2][1] = 0;
		projection_matrix.M[2][2] = (this->far_plane + this->near_plane)/(this->near_plane - this->far_plane);
		projection_matrix.M[2][3] = 2*(this->far_plane * this->near_plane) / (this->near_plane - this->far_plane);
		projection_matrix.M[3][0] = 0;
		projection_matrix.M[3][1] = 0;
		projection_matrix.M[3][2] = -1;
		projection_matrix.M[3][3] = 0;
	}
	else if (type == ORTHOGRAPHIC) {
		projection_matrix.M[0][0] = 2 / (this->right - this->left);
		projection_matrix.M[0][1] = 0;
		projection_matrix.M[0][2] = 0;
		projection_matrix.M[0][3] = -(this->right + this->left)/(this->right - this->left);
		projection_matrix.M[1][0] = 0;
		projection_matrix.M[1][1] = 2 / (this->top - this->bottom);
		projection_matrix.M[1][2] = 0;
		projection_matrix.M[1][3] = -(this->top + this->bottom) / (this->top - this->bottom);;
		projection_matrix.M[2][0] = 0;
		projection_matrix.M[2][1] = 0;
		projection_matrix.M[2][2] = -2 / (this->far_plane - this->near_plane);
		projection_matrix.M[2][3] = -(this->far_plane + this->near_plane) / (this->far_plane - this->near_plane);;
		projection_matrix.M[3][0] = 0;
		projection_matrix.M[3][1] = 0;
		projection_matrix.M[3][2] = 0;
		projection_matrix.M[3][3] = -1;
	} 

	UpdateViewProjectionMatrix();
}

void Camera::UpdateViewProjectionMatrix()
{
	viewprojection_matrix = projection_matrix * view_matrix;
}

Matrix44 Camera::GetViewProjectionMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	return viewprojection_matrix;
}

// The following methods have been created for testing.
// Do not modify them.

void Camera::SetExampleViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.m );
}

void Camera::SetExampleProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (type == PERSPECTIVE)
		gluPerspective(fov, aspect, near_plane, far_plane);
	else
		glOrtho(left,right,bottom,top,near_plane,far_plane);

	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.m );
	glMatrixMode(GL_MODELVIEW);
}
