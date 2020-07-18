/**
 * \file
 * \brief SignalInformation class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SIGNALINFORMATION_HPP_
#define INCLUDE_DISTORTOS_SIGNALINFORMATION_HPP_

#include <csignal>
#include <cstdint>

namespace distortos
{

/**
 * \brief SignalInformation class contains information about queued signal
 *
 * Similar to siginfo_t - http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/signal.h.html
 *
 * \ingroup signals
 */

class SignalInformation
{
public:

	/// replacement for predefined \a si_code values
	enum class Code : uint8_t
	{
		/// signal generated by Thread::generateSignal() or ThisThread::Signals::generateSignal() (similar to
		/// \a SI_USER)
		generated,
		/// signal queued by Thread::queueSignal() or ThisThread::Signals::queueSignal() (similar to \a SI_QUEUE)
		queued,
	};

	/**
	 * \brief SignalInformation's constructor.
	 *
	 * \param [in] signalNumber is the signal number (similar to \a si_signo member of \a siginfo_t)
	 * \param [in] code is the signal code (similar to \a si_code member of \a siginfo_t)
	 * \param [in] value is the signal value (similar to \a si_value member of \a siginfo_t)
	 */

	constexpr SignalInformation(const uint8_t signalNumber, const Code code, const sigval value) :
			value_(value),
			code_{code},
			signalNumber_{signalNumber}
	{

	}

	/**
	 * \return signal code (similar to \a si_code member of \a siginfo_t)
	 */

	Code getCode() const
	{
		return code_;
	}

	/**
	 * \return signal number (similar to \a si_signo member of \a siginfo_t)
	 */

	uint8_t getSignalNumber() const
	{
		return signalNumber_;
	}

	/**
	 * \return signal value (similar to \a si_value member of \a siginfo_t)
	 */

	sigval getValue() const
	{
		return value_;
	}

private:

	/// signal value (similar to \a si_value member of \a siginfo_t)
	sigval value_;

	/// signal code (similar to \a si_code member of \a siginfo_t)
	Code code_;

	/// signal number (similar to \a si_signo member of \a siginfo_t)
	uint8_t signalNumber_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALINFORMATION_HPP_
