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


#define LOG_TRACE(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::trace) << logEvent;

#define LOG_DEBUG(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::debug) << logEvent;

#define LOG_INFO(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::info) << logEvent;

#define LOG_WARN(logEvent)   BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::warning) << logEvent;

#define LOG_ERROR(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::error) << logEvent;

#define LOG_FATAL(logEvent)  BOOST_LOG_FUNCTION(); BOOST_LOG_SEV(Logger::Instance()._logger, boost::log::trivial::fatal) << logEvent;


void initLogger();
// https://www.boost.org/doc/libs/1_79_0/libs/log/doc/html/index.html