// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ios/chrome/browser/payments/payment_method_selection_coordinator.h"

#include "components/autofill/core/browser/credit_card.h"
#include "ios/chrome/browser/payments/payment_request.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@interface PaymentMethodSelectionCoordinator () {
  CreditCardEditCoordinator* _creditCardEditCoordinator;
}

@property(nonatomic, strong)
    PaymentMethodSelectionViewController* viewController;

// Called when the user selects a payment method. The cell is checked, the
// UI is locked so that the user can't interact with it, then the delegate is
// notified. The delay is here to let the user get a visual feedback of the
// selection before this view disappears.
- (void)delayedNotifyDelegateOfSelection:(autofill::CreditCard*)paymentMethod;

@end

@implementation PaymentMethodSelectionCoordinator
@synthesize paymentRequest = _paymentRequest;
@synthesize delegate = _delegate;
@synthesize viewController = _viewController;

- (void)start {
  _viewController = [[PaymentMethodSelectionViewController alloc]
      initWithPaymentRequest:_paymentRequest];
  [_viewController setDelegate:self];
  [_viewController loadModel];

  DCHECK([self baseViewController].navigationController);
  [[self baseViewController].navigationController
      pushViewController:_viewController
                animated:YES];
}

- (void)stop {
  [[self baseViewController].navigationController
      popViewControllerAnimated:YES];
  [_creditCardEditCoordinator stop];
  _creditCardEditCoordinator = nil;
  _viewController = nil;
}

#pragma mark - PaymentMethodSelectionViewControllerDelegate

- (void)paymentMethodSelectionViewController:
            (PaymentMethodSelectionViewController*)controller
                      didSelectPaymentMethod:
                          (autofill::CreditCard*)paymentMethod {
  [self delayedNotifyDelegateOfSelection:paymentMethod];
}

- (void)paymentMethodSelectionViewControllerDidReturn:
    (PaymentMethodSelectionViewController*)controller {
  [_delegate paymentMethodSelectionCoordinatorDidReturn:self];
}

- (void)delayedNotifyDelegateOfSelection:(autofill::CreditCard*)paymentMethod {
  _viewController.view.userInteractionEnabled = NO;
  __weak PaymentMethodSelectionCoordinator* weakSelf = self;
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW,
                               static_cast<int64_t>(0.2 * NSEC_PER_SEC)),
                 dispatch_get_main_queue(), ^{
                   PaymentMethodSelectionCoordinator* strongSelf = weakSelf;
                   // Early return if the coordinator has been deallocated.
                   if (!strongSelf)
                     return;

                   strongSelf.viewController.view.userInteractionEnabled = YES;
                   [strongSelf.delegate
                       paymentMethodSelectionCoordinator:strongSelf
                                  didSelectPaymentMethod:paymentMethod];
                 });
}

- (void)paymentMethodSelectionViewControllerDidSelectAddCard:
    (PaymentMethodSelectionViewController*)controller {
  _creditCardEditCoordinator = [[CreditCardEditCoordinator alloc]
      initWithBaseViewController:_viewController];
  [_creditCardEditCoordinator setPaymentRequest:_paymentRequest];
  [_creditCardEditCoordinator setDelegate:self];
  [_creditCardEditCoordinator start];
}

#pragma mark - CreditCardEditCoordinatorDelegate

- (void)creditCardEditCoordinator:(CreditCardEditCoordinator*)coordinator
       didFinishEditingCreditCard:(autofill::CreditCard*)creditCard {
  [_creditCardEditCoordinator stop];
  _creditCardEditCoordinator = nil;

  // Inform |_delegate| that this card has been selected.
  [_delegate paymentMethodSelectionCoordinator:self
                        didSelectPaymentMethod:creditCard];
}

- (void)creditCardEditCoordinatorDidCancel:
    (CreditCardEditCoordinator*)coordinator {
  [_creditCardEditCoordinator stop];
  _creditCardEditCoordinator = nil;
}

@end
