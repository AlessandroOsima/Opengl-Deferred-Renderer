#include "stdafx.h"
#include "Framebuffer.h"
#include "Renderer/GLUtilities.h"


Framebuffer::Framebuffer()
{
	glCheckFunction(glCreateFramebuffers(1, &OffscreenFramebufferID));
}

Framebuffer::~Framebuffer()
{
	glCheckFunction(glDeleteFramebuffers(1, &OffscreenFramebufferID));
}

void Framebuffer::BindFramebuffer(FramebufferBindType BindType)
{
	BindingTarget = BindType;

	glBindFramebuffer(static_cast<GLenum>(BindingTarget), OffscreenFramebufferID);
}

void Framebuffer::UnBindFramebuffer()
{
	glBindFramebuffer(static_cast<GLenum>(BindingTarget), 0);
}

bool Framebuffer::IsComplete()
{
	GLint boundFB = 0;

	switch (BindingTarget)
	{
	case FramebufferBindType::READ:
	{
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &boundFB);

		break;
	}
	case FramebufferBindType::DRAW:
	{
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &boundFB);

		break;
	}
	case FramebufferBindType::FRAMEBUFFER:
	{
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &boundFB);

		break;
	}
	}
	bool checked = false;

	if (boundFB && boundFB == OffscreenFramebufferID)
	{
		GLenum res = glCheckNamedFramebufferStatus(OffscreenFramebufferID, static_cast<GLenum>(BindingTarget));
		if (res == GL_FRAMEBUFFER_COMPLETE)
		{
			checked = true;
		}
	}

	return checked;
}

void Framebuffer::BindTextureToFramebuffer(const Texture & TextureToBind, unsigned int AttachmentPoint, uint32_t Level)
{
	glNamedFramebufferTexture(OffscreenFramebufferID, AttachmentPoint, TextureToBind.GetID(), Level);
}

void Framebuffer::UnbindFramebufferAttachment(unsigned int AttachmentPoint)
{
	glNamedFramebufferTexture(OffscreenFramebufferID, AttachmentPoint, 0, 0);
}
