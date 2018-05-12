#pragma once

#include <nlohmann/json.hpp>

#include <glm/glm.hpp>


// partial specialization (full specialization works too)
namespace nlohmann {
	template <>
	struct adl_serializer<glm::vec2>
	{
		static void to_json(json& j, const glm::vec2& v)
		{
			j = nlohmann::json{
				{ "x", v.x },
				{ "y", v.y }
			};
		}

		static void from_json(const json& j, glm::vec2& v)
		{
			v.x = j.at("x").get<float>();
			v.y = j.at("y").get<float>();
		}
	};

	template <>
	struct adl_serializer<glm::vec3>
	{
		static void to_json(json& j, const glm::vec3& vect)
		{
			j = nlohmann::json
			{
				{ "x", vect.x },
				{ "y", vect.y },
				{ "z", vect.z }
			};
		}

		static void from_json(const json& j, glm::vec3& v)
		{
			v.x = j.at("x").get<float>();
			v.y = j.at("y").get<float>();
			v.z = j.at("z").get<float>();
		}
	};

	template <>
	struct adl_serializer<glm::mat4>
	{
		static void to_json(json& j, const glm::mat4& m)
		{
			j = nlohmann::json
			{
				{ "00", m[0][0] },{ "01", m[0][1] },{ "02", m[0][2] },{ "03", m[0][3] },
				{ "10", m[1][0] },{ "11", m[1][1] },{ "12", m[1][2] },{ "13", m[1][3] },
				{ "20", m[2][0] },{ "21", m[2][1] },{ "22", m[2][2] },{ "23", m[2][3] },
				{ "30", m[3][0] },{ "31", m[3][1] },{ "32", m[3][2] },{ "33", m[3][3] }
			};
		}

		static void from_json(const json& j, glm::mat4& m)
		{
			m[0][0] = j.at("00").get<float>();
			m[0][1] = j.at("00").get<float>();
			m[0][2] = j.at("00").get<float>();
			m[0][3] = j.at("00").get<float>();

			m[1][0] = j.at("10").get<float>();
			m[1][1] = j.at("11").get<float>();
			m[1][2] = j.at("12").get<float>();
			m[1][3] = j.at("13").get<float>();

			m[2][0] = j.at("20").get<float>();
			m[2][1] = j.at("21").get<float>();
			m[2][2] = j.at("22").get<float>();
			m[2][3] = j.at("23").get<float>();

			m[3][0] = j.at("30").get<float>();
			m[3][1] = j.at("31").get<float>();
			m[3][2] = j.at("32").get<float>();
			m[3][3] = j.at("33").get<float>();
		}
	};
}



namespace rain
{
	/*void to_json(nlohmann::json& j, const glm::vec3& vect)
	{
		j = nlohmann::json{ { "x", vect.x },{ "y", vect.y },{ "z", vect.z } };
	}

	void from_json(const nlohmann::json& j, glm::vec3& vect)
	{
		vect.x = j.at("x").get<float>();
		vect.y = j.at("y").get<float>();
		vect.z = j.at("z").get<float>();
	}

	void to_json(nlohmann::json& j, const glm::vec2& vect)
	{
		j = nlohmann::json{ { "x", vect.x },{ "y", vect.y } };
	}

	void from_json(const nlohmann::json& j, glm::vec2& vect)
	{
		vect.x = j.at("x").get<float>();
		vect.y = j.at("y").get<float>();
	}

	void to_json(nlohmann::json& j, const glm::mat4& vect)
	{
		j = nlohmann::json
		{
			{ "00", vect[0][0] },{ "01", vect[0][1] },{ "02", vect[0][2] },{ "03", vect[0][3] },
			{ "10", vect[1][0] },{ "11", vect[1][1] },{ "12", vect[1][2] },{ "13", vect[1][3] },
			{ "20", vect[2][0] },{ "21", vect[2][1] },{ "22", vect[2][2] },{ "23", vect[2][3] },
			{ "30", vect[3][0] },{ "31", vect[3][1] },{ "32", vect[3][2] },{ "33", vect[3][3] }
		};
	}

	void from_json(const nlohmann::json& j, glm::mat4& vect)
	{
		vect[0][0] = j.at("00").get<float>();
		vect[0][1] = j.at("00").get<float>();
		vect[0][2] = j.at("00").get<float>();
		vect[0][3] = j.at("00").get<float>();

		vect[1][0] = j.at("10").get<float>();
		vect[1][1] = j.at("11").get<float>();
		vect[1][2] = j.at("12").get<float>();
		vect[1][3] = j.at("13").get<float>();

		vect[2][0] = j.at("20").get<float>();
		vect[2][1] = j.at("21").get<float>();
		vect[2][2] = j.at("22").get<float>();
		vect[2][3] = j.at("23").get<float>();

		vect[3][0] = j.at("30").get<float>();
		vect[3][1] = j.at("31").get<float>();
		vect[3][2] = j.at("32").get<float>();
		vect[3][3] = j.at("33").get<float>();
	}*/
}