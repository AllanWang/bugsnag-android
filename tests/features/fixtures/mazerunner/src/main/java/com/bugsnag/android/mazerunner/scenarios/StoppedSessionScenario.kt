package com.bugsnag.android.mazerunner.scenarios

import android.content.Context
import com.bugsnag.android.Bugsnag
import com.bugsnag.android.Configuration
import com.bugsnag.android.flushAllSessions
import android.os.Handler
import android.os.HandlerThread

/**
 * Sends an exception after stopping the session
 */
internal class StoppedSessionScenario(config: Configuration,
                                      context: Context) : Scenario(config, context) {
    override fun run() {
        super.run()
        val client = Bugsnag.getClient()
        val thread = HandlerThread("HandlerThread")
        thread.start()

        Handler(thread.looper).post {
            // send 1st exception which should include session info
            client.startSession()
            flushAllSessions()
            client.notifyBlocking(generateException())

            // send 2nd exception which should not include session info
            client.stopSession()
            flushAllSessions()
            client.notifyBlocking(generateException())
        }
    }
}
