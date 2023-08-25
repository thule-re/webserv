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
		content += "\n" + line;
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
		case 200: return MSG_200;
		case 201: return MSG_201;
		case 202: return MSG_202;
		case 204: return MSG_204;
		case 206: return MSG_206;
		case 300: return MSG_300;
		case 301: return MSG_301;
		case 302: return MSG_302;
		case 303: return MSG_303;
		case 304: return MSG_304;
		case 305: return MSG_305;
		case 307: return MSG_307;
		case 400: return MSG_400;
		case 401: return MSG_401;
		case 402: return MSG_402;
		case 403: return MSG_403;
		case 404: return MSG_404;
		case 405: return MSG_405;
		case 406: return MSG_406;
		case 407: return MSG_407;
		case 408: return MSG_408;
		case 409: return MSG_409;
		case 410: return MSG_410;
		case 411: return MSG_411;
		case 412: return MSG_412;
		case 413: return MSG_413;
		case 414: return MSG_414;
		case 415: return MSG_415;
		case 416: return MSG_416;
		case 417: return MSG_417;
		case 500: return MSG_500;
		case 501: return MSG_501;
		case 502: return MSG_502;
		case 503: return MSG_503;
		case 504: return MSG_504;
		case 505: return MSG_505;
		default: return "Unknown HTTP Error";
	}
}
