/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseCodes.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: treeps <treeps@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 17:51:38 by treeps            #+#    #+#             */
/*   Updated: 2023/08/21 17:51:38 by treeps           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_RESPONSECODES_HPP
# define WEBSERV_RESPONSECODES_HPP

// HTTP Status Codes
// 2xx Success
# define OK 200
# define CREATED 201
# define ACCEPTED 202
# define NO_CONTENT 204
# define PARTIAL_CONTENT 206

// 3xx Redirection
# define MULTIPLE_CHOICES 300
# define MOVED_PERMANENTLY 301
# define FOUND 302
# define SEE_OTHER 303
# define NOT_MODIFIED 304
# define USE_PROXY 305
# define TEMPORARY_REDIRECT 307
# define PERMANENT_REDIRECT 308

// 4xx Client Error
# define BAD_REQUEST 400
# define UNAUTHORIZED 401
# define PAYMENT_REQUIRED 402
# define FORBIDDEN 403
# define NOT_FOUND 404
# define METHOD_NOT_ALLOWED 405
# define NOT_ACCEPTABLE 406
# define PROXY_AUTHENTICATION_REQUIRED 407
# define REQUEST_TIMEOUT 408
# define CONFLICT 409
# define GONE 410
# define LENGTH_REQUIRED 411
# define PRECONDITION_FAILED 412
# define REQUEST_ENTITY_TOO_LARGE 413
# define REQUEST_URI_TOO_LONG 414
# define UNSUPPORTED_MEDIA_TYPE 415
# define REQUESTED_RANGE_NOT_SATISFIABLE 416
# define EXPECTATION_FAILED 417
# define IM_A_TEAPOT 418

// 5xx Server Error
# define INTERNAL_SERVER_ERROR 500
# define NOT_IMPLEMENTED 501
# define BAD_GATEWAY 502
# define SERVICE_UNAVAILABLE 503
# define GATEWAY_TIMEOUT 504
# define HTTP_VERSION_NOT_SUPPORTED 505

// HTTP Error Messages
// 2xx Success
# define MSG_200 "OK"
# define MSG_201 "Created"
# define MSG_202 "Accepted"
# define MSG_204 "No Content"
# define MSG_206 "Partial Content"

// 3xx Redirection
# define MSG_300 "Multiple Choices"
# define MSG_301 "Moved Permanently"
# define MSG_302 "Found"
# define MSG_303 "See Other"
# define MSG_304 "Not Modified"
# define MSG_305 "Use Proxy"
# define MSG_307 "Temporary Redirect"
# define MSG_308 "Permanent Redirect"

// 4xx Client Error
# define MSG_400 "Bad Request"
# define MSG_401 "Unauthorized"
# define MSG_402 "Payment Required"
# define MSG_403 "Forbidden"
# define MSG_404 "Not Found"
# define MSG_405 "Method Not Allowed"
# define MSG_406 "Not Acceptable"
# define MSG_407 "Proxy Authentication Required"
# define MSG_408 "Request Timeout"
# define MSG_409 "Conflict"
# define MSG_410 "Gone"
# define MSG_411 "Length Required"
# define MSG_412 "Precondition Failed"
# define MSG_413 "Request Entity Too Large"
# define MSG_414 "Request-URI Too Long"
# define MSG_415 "Unsupported Media Type"
# define MSG_416 "Requested Range Not Satisfiable"
# define MSG_417 "Expectation Failed"
# define MSG_418 "I'm a teapot"

// 5xx Server Error
# define MSG_500 "Internal Server Error"
# define MSG_501 "Not Implemented"
# define MSG_502 "Bad Gateway"
# define MSG_503 "Service Unavailable"
# define MSG_504 "Gateway Timeout"
# define MSG_505 "HTTP Version Not Supported"

#endif
