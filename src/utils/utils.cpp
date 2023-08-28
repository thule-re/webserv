/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 19:20:33 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 19:20:33 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.hpp"

void replaceAll(std::string &str, const std::string &oldStr, const std::string &newStr) {
	size_t pos = 0;
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

std::string readFile(std::ifstream &file) {
	std::string content;
	std::string line;
	getline(file, line);
	content += line;
	while (getline(file, line))
		content += "\r\n" + line;
	return (content);
}

std::string readFile(const std::string &path) {
	std::ifstream file(path.c_str());
	if (!file.is_open())
		return ("");
	std::string content = readFile(file);
	file.close();
	return (content);
}

std::string getContentType(const std::string& path) {
	if (path.find(".html") != std::string::npos) {
		return "text/html";
	} else if (path.find(".css") != std::string::npos) {
		return "text/css";
	} else if (path.find(".js") != std::string::npos) {
		return "text/javascript";
	} else if (path.find(".png") != std::string::npos) {
		return "image/png";
	} else if (path.find(".jpg") != std::string::npos) {
		return "image/jpeg";
	} else if (path.find(".gif") != std::string::npos) {
		return "image/gif";
	} else if (path.find(".ico") != std::string::npos) {
		return "image/x-icon";
	} else {
		return "text/plain";
	}
}

std::string getHTTPErrorMessages(int statusCode) {
	switch (statusCode) {
		case OK:								return MSG_200;
		case CREATED:							return MSG_201;
		case ACCEPTED:							return MSG_202;
		case NO_CONTENT:						return MSG_204;
		case PARTIAL_CONTENT:					return MSG_206;
		case MULTIPLE_CHOICES:					return MSG_300;
		case MOVED_PERMANENTLY:					return MSG_301;
		case FOUND:								return MSG_302;
		case SEE_OTHER:							return MSG_303;
		case NOT_MODIFIED:						return MSG_304;
		case USE_PROXY:							return MSG_305;
		case TEMPORARY_REDIRECT:				return MSG_307;
		case BAD_REQUEST:						return MSG_400;
		case UNAUTHORIZED:						return MSG_401;
		case PAYMENT_REQUIRED:					return MSG_402;
		case FORBIDDEN:							return MSG_403;
		case NOT_FOUND:							return MSG_404;
		case METHOD_NOT_ALLOWED:				return MSG_405;
		case NOT_ACCEPTABLE:					return MSG_406;
		case PROXY_AUTHENTICATION_REQUIRED:		return MSG_407;
		case REQUEST_TIMEOUT:					return MSG_408;
		case CONFLICT:							return MSG_409;
		case GONE:								return MSG_410;
		case LENGTH_REQUIRED:					return MSG_411;
		case PRECONDITION_FAILED:				return MSG_412;
		case REQUEST_ENTITY_TOO_LARGE:			return MSG_413;
		case REQUEST_URI_TOO_LONG:				return MSG_414;
		case UNSUPPORTED_MEDIA_TYPE:			return MSG_415;
		case REQUESTED_RANGE_NOT_SATISFIABLE:	return MSG_416;
		case EXPECTATION_FAILED:				return MSG_417;
		case INTERNAL_SERVER_ERROR:				return MSG_500;
		case NOT_IMPLEMENTED:					return MSG_501;
		case BAD_GATEWAY:						return MSG_502;
		case SERVICE_UNAVAILABLE:				return MSG_503;
		case GATEWAY_TIMEOUT:					return MSG_504;
		case HTTP_VERSION_NOT_SUPPORTED:		return MSG_505;
		default: return "Unknown HTTP Error";
	}
}
