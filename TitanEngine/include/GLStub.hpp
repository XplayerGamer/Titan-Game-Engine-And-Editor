// Minimal GL stubs to allow compilation on systems without a GL loader.
// These are no-op implementations intended only to make the project build;
// they do not provide real rendering functionality.
#pragma once

#include <cstdint>

using GLuint = unsigned int;
using GLenum = unsigned int;
using GLsizei = int;
using GLint = int;
using GLboolean = unsigned char;

// Common enums used by our code
constexpr GLenum GL_ARRAY_BUFFER = 0x8892u;
constexpr GLenum GL_DYNAMIC_DRAW = 0x88E8u;
constexpr GLenum GL_FLOAT = 0x1406u;
constexpr GLenum GL_FALSE = 0u;
constexpr GLenum GL_TEXTURE_2D = 0x0DE1u;
constexpr GLenum GL_DEPTH_COMPONENT = 0x1902u;
constexpr GLenum GL_TEXTURE_MIN_FILTER = 0x2801u;
constexpr GLenum GL_TEXTURE_MAG_FILTER = 0x2800u;
constexpr GLenum GL_LINEAR = 0x2601u;
constexpr GLenum GL_COLOR_BUFFER_BIT = 0x00004000u;
constexpr GLenum GL_DEPTH_BUFFER_BIT = 0x00000100u;
constexpr GLenum GL_FRAMEBUFFER = 0x8D40u;
constexpr GLenum GL_POINTS = 0x0000u;

inline void glGenVertexArrays(GLsizei n, GLuint* arrays) { for (GLsizei i=0;i<n;i++) arrays[i]=0; }
inline void glGenBuffers(GLsizei n, GLuint* buffers) { for (GLsizei i=0;i<n;i++) buffers[i]=0; }
inline void glBindVertexArray(GLuint array) { (void)array; }
inline void glBindBuffer(GLenum target, GLuint buffer) { (void)target; (void)buffer; }
inline void glBufferData(GLenum target, std::size_t size, const void* data, GLenum usage) { (void)target; (void)size; (void)data; (void)usage; }
inline void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) { (void)index; (void)size; (void)type; (void)normalized; (void)stride; (void)pointer; }
inline void glEnableVertexAttribArray(GLuint index) { (void)index; }
inline void glBufferSubData(GLenum target, std::size_t offset, std::size_t size, const void* data) { (void)target; (void)offset; (void)size; (void)data; }
inline void glDrawArrays(GLenum mode, GLint first, GLsizei count) { (void)mode; (void)first; (void)count; }

inline void glGenFramebuffers(GLsizei n, GLuint* ids) { for (GLsizei i=0;i<n;i++) ids[i]=0; }
inline void glGenTextures(GLsizei n, GLuint* ids) { for (GLsizei i=0;i<n;i++) ids[i]=0; }
inline void glBindTexture(GLenum target, GLuint tex) { (void)target; (void)tex; }
inline void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* data) { (void)target; (void)level; (void)internalFormat; (void)width; (void)height; (void)border; (void)format; (void)type; (void)data; }
inline void glTexParameteri(GLenum target, GLenum pname, GLint param) { (void)target; (void)pname; (void)param; }
inline void glDeleteVertexArrays(GLsizei n, const GLuint* arrays) { (void)n; (void)arrays; }
inline void glDeleteBuffers(GLsizei n, const GLuint* buffers) { (void)n; (void)buffers; }
inline void glDeleteTextures(GLsizei n, const GLuint* textures) { (void)n; (void)textures; }
inline void glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers) { (void)n; (void)framebuffers; }
inline void glClearColor(float r, float g, float b, float a) { (void)r; (void)g; (void)b; (void)a; }
inline void glClear(GLenum mask) { (void)mask; }
inline void glBindFramebuffer(GLenum target, GLuint framebuffer) { (void)target; (void)framebuffer; }
