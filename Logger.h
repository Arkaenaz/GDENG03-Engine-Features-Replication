#pragma once

#include <Windows.h>

#include <string>
#include <iostream>
#include <exception>

#define DEBUG_LOGS true
#define ERROR_LOGS true
#define SUCCESS_LOGS false

namespace GDEngine
{
	class Logger
	{
	public:
		template <class T>
		static void log(T* sender, const std::string& message)
		{
			if (!DEBUG_LOGS)
				return;

			std::cout << "[" << getType(sender) << " DEBUG]: " << message << '\n';
		}

		static void log(const std::string& message)
		{
			if (!DEBUG_LOGS)
				return;

			std::cout << "[DEBUG]: " << message << '\n';
		}

		static void log(const std::wstring& message)
		{
			if (!DEBUG_LOGS)
				return;

			std::wcout << "[DEBUG]: " << message << '\n';
		}

		template <class T>
		static bool log(T* sender, const HRESULT result)
		{
			if (FAILED(result)) {
				const std::string message = std::system_category().message(result);
				log(sender, message);
				return false;
			}

			if (SUCCESS_LOGS)
				log(sender, "Operation was successful.");

			return true;
		}

		template <class T>
		static void error(T* sender, const std::string& msg)
		{
			if (!ERROR_LOGS)
				return;

			std::cout << "[" << getType(sender) << " ERROR]: " << msg << '\n';
		}

		static void throw_exception(const std::string& msg)
		{
			std::cout << "[EXCEPTION]: " << msg << '\n';
			throw std::exception(msg.c_str());
		}

		static void out_of_range(const std::string& msg)
		{
			std::cout << "[OUT OF RANGE]: " << msg << '\n';
			throw std::out_of_range(msg.c_str());
		}

	private:
		template <class T>
		static std::string getType(T* type)
		{
			return typeid(*type).name();
		}
	};
}

