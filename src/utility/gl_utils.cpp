#include "gl_utils.h"


namespace rain
{

	std::unordered_map<std::string, GLenum> GLUtils::m_stringToGLType =
	{
		{ "float"						, GL_FLOAT },
		{ "vec2"						, GL_FLOAT_VEC2 },
		{ "vec3"						, GL_FLOAT_VEC3 },
		{ "vec4"						, GL_FLOAT_VEC4 },
		{ "double"						, GL_DOUBLE },
		{ "dvec2"						, GL_DOUBLE_VEC2 },
		{ "dvec3"						, GL_DOUBLE_VEC3 },
		{ "dvec4"						, GL_DOUBLE_VEC4 },
		{ "int"							, GL_INT },
		{ "ivec2"						, GL_INT_VEC2 },
		{ "ivec3"						, GL_INT_VEC3 },
		{ "ivec4"						, GL_INT_VEC4 },
		{ "unsigned int"				, GL_UNSIGNED_INT },
		{ "uvec2"						, GL_UNSIGNED_INT_VEC2 },
		{ "uvec3"						, GL_UNSIGNED_INT_VEC3 },
		{ "uvec4"						, GL_UNSIGNED_INT_VEC4 },
		{ "bool"						, GL_BOOL },
		{ "bvec2"						, GL_BOOL_VEC2 },
		{ "bvec3"						, GL_BOOL_VEC3 },
		{ "bvec4"						, GL_BOOL_VEC4 },
		{ "mat2"						, GL_FLOAT_MAT2 },
		{ "mat3"						, GL_FLOAT_MAT3 },
		{ "mat4"						, GL_FLOAT_MAT4 },
		{ "mat2x3"						, GL_FLOAT_MAT2x3 },
		{ "mat2x4"						, GL_FLOAT_MAT2x4 },
		{ "mat3x2"						, GL_FLOAT_MAT3x2 },
		{ "mat3x4"						, GL_FLOAT_MAT3x4 },
		{ "mat4x2"						, GL_FLOAT_MAT4x2 },
		{ "mat4x3"						, GL_FLOAT_MAT4x3 },
		{ "dmat2"						, GL_DOUBLE_MAT2 },
		{ "dmat3"						, GL_DOUBLE_MAT3 },
		{ "dmat4"						, GL_DOUBLE_MAT4 },
		{ "dmat2x3"						, GL_DOUBLE_MAT2x3 },
		{ "dmat2x4"						, GL_DOUBLE_MAT2x4 },
		{ "dmat3x2"						, GL_DOUBLE_MAT3x2 },
		{ "dmat3x4"						, GL_DOUBLE_MAT3x4 },
		{ "dmat4x2"						, GL_DOUBLE_MAT4x2 },
		{ "dmat4x3"						, GL_DOUBLE_MAT4x3 },
		{ "sampler1D"					, GL_SAMPLER_1D },
		{ "sampler2D"					, GL_SAMPLER_2D },
		{ "sampler3D"					, GL_SAMPLER_3D },
		{ "samplerCube"					, GL_SAMPLER_CUBE },
		{ "sampler1DShadow"				, GL_SAMPLER_1D_SHADOW },
		{ "sampler2DShadow"				, GL_SAMPLER_2D_SHADOW },
		{ "sampler1DArray"				, GL_SAMPLER_1D_ARRAY },
		{ "sampler2DArray"				, GL_SAMPLER_2D_ARRAY },
		{ "sampler1DArrayShadow"		, GL_SAMPLER_1D_ARRAY_SHADOW },
		{ "sampler2DArrayShadow"		, GL_SAMPLER_2D_ARRAY_SHADOW },
		{ "sampler2DMS"					, GL_SAMPLER_2D_MULTISAMPLE },
		{ "sampler2DMSArray"			, GL_SAMPLER_2D_MULTISAMPLE_ARRAY },
		{ "samplerCubeShadow"			, GL_SAMPLER_CUBE_SHADOW },
		{ "samplerBuffer"				, GL_SAMPLER_BUFFER },
		{ "sampler2DRect"				, GL_SAMPLER_2D_RECT },
		{ "sampler2DRectShadow"			, GL_SAMPLER_2D_RECT_SHADOW },
		{ "isampler1D"					, GL_INT_SAMPLER_1D },
		{ "isampler2D"					, GL_INT_SAMPLER_2D },
		{ "isampler3D"					, GL_INT_SAMPLER_3D },
		{ "isamplerCube"				, GL_INT_SAMPLER_CUBE },
		{ "isampler1DArray"				, GL_INT_SAMPLER_1D_ARRAY },
		{ "isampler2DArray"				, GL_INT_SAMPLER_2D_ARRAY },
		{ "isampler2DMS"				, GL_INT_SAMPLER_2D_MULTISAMPLE },
		{ "isampler2DMSArray"			, GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY },
		{ "isamplerBuffer"				, GL_INT_SAMPLER_BUFFER },
		{ "isampler2DRect"				, GL_INT_SAMPLER_2D_RECT },
		{ "usampler1D"					, GL_UNSIGNED_INT_SAMPLER_1D },
		{ "usampler2D"					, GL_UNSIGNED_INT_SAMPLER_2D },
		{ "usampler3D"					, GL_UNSIGNED_INT_SAMPLER_3D },
		{ "usamplerCube"				, GL_UNSIGNED_INT_SAMPLER_CUBE },
		{ "usampler2DArray"				, GL_UNSIGNED_INT_SAMPLER_1D_ARRAY },
		{ "usampler2DArray"				, GL_UNSIGNED_INT_SAMPLER_2D_ARRAY },
		{ "usampler2DMS"				, GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE },
		{ "usampler2DMSArray"			, GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY },
		{ "usamplerBuffer"				, GL_UNSIGNED_INT_SAMPLER_BUFFER },
		{ "usampler2DRect"				, GL_UNSIGNED_INT_SAMPLER_2D_RECT },
		{ "image1D"						, GL_IMAGE_1D },
		{ "image2D"						, GL_IMAGE_2D },
		{ "image3D"						, GL_IMAGE_3D },
		{ "image2DRect"					, GL_IMAGE_2D_RECT },
		{ "imageCube"					, GL_IMAGE_CUBE },
		{ "imageBuffer"					, GL_IMAGE_BUFFER },
		{ "image1DArray"				, GL_IMAGE_1D_ARRAY },
		{ "image2DArray"				, GL_IMAGE_2D_ARRAY },
		{ "image2DMS"					, GL_IMAGE_2D_MULTISAMPLE },
		{ "image2DMSArray"				, GL_IMAGE_2D_MULTISAMPLE_ARRAY },
		{ "iimage1D"					, GL_INT_IMAGE_1D },
		{ "iimage2D"					, GL_INT_IMAGE_2D },
		{ "iimage3D"					, GL_INT_IMAGE_3D },
		{ "iimage2DRect"				, GL_INT_IMAGE_2D_RECT },
		{ "iimageCube"					, GL_INT_IMAGE_CUBE },
		{ "iimageBuffer"				, GL_INT_IMAGE_BUFFER },
		{ "iimage1DArray"				, GL_INT_IMAGE_1D_ARRAY },
		{ "iimage2DArray"				, GL_INT_IMAGE_2D_ARRAY },
		{ "iimage2DMS"					, GL_INT_IMAGE_2D_MULTISAMPLE },
		{ "iimage2DMSArray"				, GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY },
		{ "uimage1D"					, GL_UNSIGNED_INT_IMAGE_1D },
		{ "uimage2D"					, GL_UNSIGNED_INT_IMAGE_2D },
		{ "uimage3D"					, GL_UNSIGNED_INT_IMAGE_3D },
		{ "uimage2DRect"				, GL_UNSIGNED_INT_IMAGE_2D_RECT },
		{ "uimageCube"					, GL_UNSIGNED_INT_IMAGE_CUBE },
		{ "uimageBuffer"				, GL_UNSIGNED_INT_IMAGE_BUFFER },
		{ "uimage1DArray"				, GL_UNSIGNED_INT_IMAGE_1D_ARRAY },
		{ "uimage2DArray"				, GL_UNSIGNED_INT_IMAGE_2D_ARRAY },
		{ "uimage2DMS"					, GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE },
		{ "uimage2DMSArray"				, GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY },
		{ "atomic_uint"					, GL_UNSIGNED_INT_ATOMIC_COUNTER }
	};



	GLenum GLUtils::StringToGLType(const std::string& _name)
	{
		return m_stringToGLType.at(_name);
	}

    std::string GLUtils::GLTypeToString(GLenum _enum)
    {
        switch (_enum)
        {
        case GL_FLOAT:										return "float";
        case GL_FLOAT_VEC2:									return "vec2";
        case GL_FLOAT_VEC3:									return "vec3";
        case GL_FLOAT_VEC4:									return "vec4";
        case GL_DOUBLE:										return "double";
        case GL_DOUBLE_VEC2:								return "dvec2";
        case GL_DOUBLE_VEC3:								return "dvec3";
        case GL_DOUBLE_VEC4:								return "dvec4";
        case GL_INT:										return "int";
        case GL_INT_VEC2:									return "ivec2";
        case GL_INT_VEC3:									return "ivec3";
        case GL_INT_VEC4:									return "ivec4";
        case GL_UNSIGNED_INT:								return "unsigned int";
        case GL_UNSIGNED_INT_VEC2:							return "uvec2";
        case GL_UNSIGNED_INT_VEC3:							return "uvec3";
        case GL_UNSIGNED_INT_VEC4:							return "uvec4";
        case GL_BOOL:										return "bool";
        case GL_BOOL_VEC2:									return "bvec2";
        case GL_BOOL_VEC3:									return "bvec3";
        case GL_BOOL_VEC4:									return "bvec4";
        case GL_FLOAT_MAT2:									return "mat2";
        case GL_FLOAT_MAT3:									return "mat3";
        case GL_FLOAT_MAT4:									return "mat4";
        case GL_FLOAT_MAT2x3:								return "mat2x3";
        case GL_FLOAT_MAT2x4:								return "mat2x4";
        case GL_FLOAT_MAT3x2:								return "mat3x2";
        case GL_FLOAT_MAT3x4:								return "mat3x4";
        case GL_FLOAT_MAT4x2:								return "mat4x2";
        case GL_FLOAT_MAT4x3:								return "mat4x3";
        case GL_DOUBLE_MAT2:								return "dmat2";
        case GL_DOUBLE_MAT3:								return "dmat3";
        case GL_DOUBLE_MAT4:								return "dmat4";
        case GL_DOUBLE_MAT2x3:								return "dmat2x3";
        case GL_DOUBLE_MAT2x4:								return "dmat2x4";
        case GL_DOUBLE_MAT3x2:								return "dmat3x2";
        case GL_DOUBLE_MAT3x4:								return "dmat3x4";
        case GL_DOUBLE_MAT4x2:								return "dmat4x2";
        case GL_DOUBLE_MAT4x3:								return "dmat4x3";
        case GL_SAMPLER_1D:									return "sampler1D";
        case GL_SAMPLER_2D:									return "sampler2D";
        case GL_SAMPLER_3D:									return "sampler3D";
        case GL_SAMPLER_CUBE:								return "samplerCube";
        case GL_SAMPLER_1D_SHADOW:							return "sampler1DShadow";
        case GL_SAMPLER_2D_SHADOW:							return "sampler2DShadow";
        case GL_SAMPLER_1D_ARRAY:							return "sampler1DArray";
        case GL_SAMPLER_2D_ARRAY:							return "sampler2DArray";
        case GL_SAMPLER_1D_ARRAY_SHADOW:					return "sampler1DArrayShadow";
        case GL_SAMPLER_2D_ARRAY_SHADOW:					return "sampler2DArrayShadow";
        case GL_SAMPLER_2D_MULTISAMPLE:						return "sampler2DMS";
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:				return "sampler2DMSArray";
        case GL_SAMPLER_CUBE_SHADOW:						return "samplerCubeShadow";
        case GL_SAMPLER_BUFFER:								return "samplerBuffer";
        case GL_SAMPLER_2D_RECT:							return "sampler2DRect";
        case GL_SAMPLER_2D_RECT_SHADOW:						return "sampler2DRectShadow";
        case GL_INT_SAMPLER_1D:								return "isampler1D";
        case GL_INT_SAMPLER_2D:								return "isampler2D";
        case GL_INT_SAMPLER_3D:								return "isampler3D";
        case GL_INT_SAMPLER_CUBE:							return "isamplerCube";
        case GL_INT_SAMPLER_1D_ARRAY:						return "isampler1DArray";
        case GL_INT_SAMPLER_2D_ARRAY:						return "isampler2DArray";
        case GL_INT_SAMPLER_2D_MULTISAMPLE:					return "isampler2DMS";
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:			return "isampler2DMSArray";
        case GL_INT_SAMPLER_BUFFER:							return "isamplerBuffer";
        case GL_INT_SAMPLER_2D_RECT:						return "isampler2DRect";
        case GL_UNSIGNED_INT_SAMPLER_1D:					return "usampler1D";
        case GL_UNSIGNED_INT_SAMPLER_2D:					return "usampler2D";
        case GL_UNSIGNED_INT_SAMPLER_3D:					return "usampler3D";
        case GL_UNSIGNED_INT_SAMPLER_CUBE	:				return "usamplerCube";
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:				return "usampler2DArray";
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:				return "usampler2DArray";
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:		return "usampler2DMS";
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:	return "usampler2DMSArray";
        case GL_UNSIGNED_INT_SAMPLER_BUFFER:				return "usamplerBuffer";
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:				return "usampler2DRect";
        case GL_IMAGE_1D:									return "image1D";
        case GL_IMAGE_2D:									return "image2D";
        case GL_IMAGE_3D:									return "image3D";
        case GL_IMAGE_2D_RECT:								return "image2DRect";
        case GL_IMAGE_CUBE:									return "imageCube";
        case GL_IMAGE_BUFFER:								return "imageBuffer";
        case GL_IMAGE_1D_ARRAY:								return "image1DArray";
        case GL_IMAGE_2D_ARRAY:								return "image2DArray";
        case GL_IMAGE_2D_MULTISAMPLE:						return "image2DMS";
        case GL_IMAGE_2D_MULTISAMPLE_ARRAY:					return "image2DMSArray";
        case GL_INT_IMAGE_1D:								return "iimage1D";
        case GL_INT_IMAGE_2D:								return "iimage2D";
        case GL_INT_IMAGE_3D:								return "iimage3D";
        case GL_INT_IMAGE_2D_RECT:							return "iimage2DRect";
        case GL_INT_IMAGE_CUBE:								return "iimageCube";
        case GL_INT_IMAGE_BUFFER:							return "iimageBuffer";
        case GL_INT_IMAGE_1D_ARRAY:							return "iimage1DArray";
        case GL_INT_IMAGE_2D_ARRAY:							return "iimage2DArray";
        case GL_INT_IMAGE_2D_MULTISAMPLE:					return "iimage2DMS";
        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:				return "iimage2DMSArray";
        case GL_UNSIGNED_INT_IMAGE_1D:						return "uimage1D";
        case GL_UNSIGNED_INT_IMAGE_2D:						return "uimage2D";
        case GL_UNSIGNED_INT_IMAGE_3D:						return "uimage3D";
        case GL_UNSIGNED_INT_IMAGE_2D_RECT:					return "uimage2DRect";
        case GL_UNSIGNED_INT_IMAGE_CUBE:					return "uimageCube";
        case GL_UNSIGNED_INT_IMAGE_BUFFER:					return "uimageBuffer";
        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:				return "uimage1DArray";
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:				return "uimage2DArray";
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:			return "uimage2DMS";
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:	return "uimage2DMSArray";
        case GL_UNSIGNED_INT_ATOMIC_COUNTER:				return "atomic_uint";
        default: return "not a GLenum type";
        }
    }
}