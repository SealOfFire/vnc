/*
 * 日志处理
 */
#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

src::severity_logger_mt<boost::log::trivial::severity_level> lg;

#define LOG_TRACE(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::trace) << logEvent;

#define LOG_DEBUG(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::debug) << logEvent;

#define LOG_INFO(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::info) << logEvent;

#define LOG_WARN(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::warning) << logEvent;

#define LOG_ERROR(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::error) << logEvent;

#define LOG_FATAL(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::fatal) << logEvent;


void initLogger() {
#ifdef Debug
    logging::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::trace
    );
#else
    logging::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::trace
    );
#endif
    boost::log::formatter formatter = boost::log::expressions::stream
        << "["
        << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp",
            "%Y-%m-%d %H:%M:%S.%f") /*.%f*/
        << "|"
        << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>(
            "ThreadID")
        << "]["
        << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
        << "]"
        << boost::log::expressions::smessage
        << "    "
        << boost::log::expressions::format_named_scope("Scope",
            boost::log::keywords::format = "(%f:%l)",
            boost::log::keywords::iteration = boost::log::expressions::reverse,
            boost::log::keywords::depth = 1);

    auto consoleSink = logging::add_console_log();
    consoleSink->set_formatter(formatter);
    boost::log::core::get()->add_sink(consoleSink);

    logging::add_file_log(
        keywords::file_name = "sample_%N.log", // 日志文件名
        keywords::open_mode = std::ios_base::app, // 日志追加方式
        keywords::rotation_size = 1 * 1024 * 1024, // 日志文件大小，1M
        keywords::format = "[%TimeStamp%]:<%Severity%> %Message%" // 日志格式
    );

    logging::add_common_attributes();
}
// https://www.boost.org/doc/libs/1_79_0/libs/log/doc/html/index.html