#include "adlLogger.h"

//bool adlLogger::init_ = false;
//std::shared_ptr<spdlog::logger> adlLogger::info_logger_ = spdlog::basic_logger_mt("info_logger", "../logs/info.log");
//std::shared_ptr<spdlog::logger> adlLogger::error_logger_ = spdlog::basic_logger_mt("error_logger", "../logs/error.log");
//std::shared_ptr<spdlog::logger> adlLogger::console_ = spdlog::stderr_color_mt("console");

adlLogger::adlLogger()
{
}

adlLogger::~adlLogger()
{
}

void adlLogger::log_error(const std::string& error, bool log_to_file /*= false*/)
{
	console_->error(error);
	if(log_to_file)
	{
		error_logger_->error(error);
		error_logger_->error(__LINE__);
	}
}

void adlLogger::log_warning(const std::string& warning, bool log_to_file/* = false*/)
{
	console_->warn(warning);
	if (log_to_file)
	{
		warning_logger_->warn(warning);
		warning_logger_->warn(__LINE__);
	}
}

void adlLogger::log_info(const std::string& info, bool log_to_file /*= false*/)
{
	console_->info(info);
	if (log_to_file)
	{
		info_logger_->info(info);
	}
}

void adlLogger::log_info(const std::string& vector_name, adlVec3& vector, bool log_to_file/* = false*/)
{
	std::string vector_string = vector_name + ": [x: " + std::to_string(vector.x) + ", y: " + std::to_string(vector.y) + ", z: " + std::to_string(vector.z) + ", w: " + std::to_string(vector.w) + "]";
	log_info(vector_string, log_to_file);
}