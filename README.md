# Insider-multiapp

## Restrictions
As stated in our paper, we have not integrated the support of simultaneous multiple applications into Insider compiler. Instead, we provide a template that supports three concurrent applications; user can replace the placeholder kernel with their application logic.

This version is simply a **proof of concept**. Since it is not integrated with Insider compiler, user needs to manually add the reset logic into their kernels which is somehow cumbersome. For the normal use case, please adopt the [single-application version](https://github.com/zainryan/Insider) which has the full end-to-end compilation support.

## Preliminaries
The same as the [one](https://github.com/zainryan/Insider#preliminaries) in the single-application version.

## BUILD and Installation
The same as the [one](https://github.com/zainryan/Insider#build-and-installation) in the single-application version.

## Usage
