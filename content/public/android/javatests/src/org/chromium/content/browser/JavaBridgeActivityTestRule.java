// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.content.browser;

import org.junit.runner.Description;
import org.junit.runners.model.Statement;

import org.chromium.content.browser.test.util.TestCallbackHelperContainer;
import org.chromium.content_shell_apk.ContentShellActivity;
import org.chromium.content_shell_apk.ContentShellActivityTestRule;
import org.chromium.content_shell_apk.ContentShellTestCommon.TestCommonCallback;

import java.lang.annotation.Annotation;

/**
 * ActivityTestRule with common functionality for testing the Java Bridge.
 */
public class JavaBridgeActivityTestRule
        extends ContentShellActivityTestRule implements TestCommonCallback<ContentShellActivity> {
    private JavaBridgeTestCommon mTestCommon;

    public JavaBridgeActivityTestRule() {
        super();
        mTestCommon = new JavaBridgeTestCommon(this);
    }

    /**
     * Sets up the ContentView. Intended to be called from setUp().
     */
    private void setUpContentView() {
        mTestCommon.setUpContentView();
    }

    public TestCallbackHelperContainer getTestCallBackHelperContainer() {
        return mTestCommon.getTestCallBackHelperContainer();
    }

    public void executeJavaScript(String script) throws Throwable {
        mTestCommon.executeJavaScript(script);
    }

    public void injectObjectAndReload(Object object, String name) throws Exception {
        injectObjectAndReload(object, name, null);
    }

    public void injectObjectAndReload(Object object, String name,
            Class<? extends Annotation> requiredAnnotation) throws Exception {
        mTestCommon.injectObjectsAndReload(object, name, null, null, requiredAnnotation);
    }

    public void injectObjectsAndReload(final Object object1, final String name1,
            final Object object2, final String name2,
            final Class<? extends Annotation> requiredAnnotation) throws Exception {
        mTestCommon.injectObjectsAndReload(object1, name1, object2, name2, requiredAnnotation);
    }

    public void synchronousPageReload() throws Throwable {
        mTestCommon.synchronousPageReload();
    }

    @Override
    public Statement apply(final Statement base, Description desc) {
        return super.apply(new Statement() {
            @Override
            public void evaluate() throws Throwable {
                setUpContentView();
                base.evaluate();
            }
        }, desc);
    }
}
