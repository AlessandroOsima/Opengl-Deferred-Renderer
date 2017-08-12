#pragma once
#include "Renderer/Texture.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


enum class FramebufferBindType : unsigned int
{
	READ = GL_READ_FRAMEBUFFER,
	DRAW = GL_DRAW_FRAMEBUFFER,
	FRAMEBUFFER = GL_FRAMEBUFFER,
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void BindTextureToFramebuffer(const Texture & TextureToBind, unsigned int AttachmentPoint, uint32_t Level = 0);
	void UnbindFramebufferAttachment(unsigned int AttachmentPoint);
	void BindFramebuffer(FramebufferBindType BindType);
	void UnBindFramebuffer();
	bool IsComplete();

	inline uint32_t GetID() const
	{
		return OffscreenFramebufferID;
	}

	FramebufferBindType BindingTarget;


private:
	GLuint OffscreenFramebufferID;
};

