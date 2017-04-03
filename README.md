# Introduction

EmailMonkey is a module that integrates with the [MailChimp API](https://developer.mailchimp.com/documentation/mailchimp/) and the [Display Suite module](https://www.drupal.org/project/ds) to make it easy to create custom email templates and to preview, export, test, and send node content as MailChimp campaigns. Everything is configured from Drupal's web interface, and the module includes a flexible, table-based Display Suite layout. The only development required is to create CSS style sheets for the templates.

Using Display Suite view modes, EmailMonkey supports multiple email templates per content type. Also, the view mode used to export a node's content to MailChimp can be set independently of the view mode used to display its content on the site.

EmailMonkey supports specifying as many styles—in the form of CSS style sheets—as necessary per view mode, which makes it easy to reuse styles among multiple email templates and multiple content types. It also offers granular permissions to support complex workflows.

EmailMonkey also handles preprocessing tasks. It automatically rewrites all URL paths to make them absolute, inlines CSS, and minifies HTML before exporting to MailChimp.

# Related Modules

- [MailChimp](https://www.drupal.org/project/mailchimp)
- [MailChimp eCommerce](https://www.drupal.org/project/mailchimp_ecommerce)
- [Mandrill](https://www.drupal.org/project/mandrill)

## Comparison to the MailChimp Module

The MailChimp module acts more like a MailChimp client, providing access to various MailChimp features from within Drupal, and its email campaign functionality is inflexible. EmailMonkey is kind of the reverse: it allows users to build flexible and complex email templates and campaigns using a wide range of Drupal components, and, from that foundation, it allows users to then send those campaigns via the MailChimp API.

EmailMonkey and the MailChimp module can coexist on the same site without any issues.

# Installation

Make sure to install the [Libraries](https://www.drupal.org/project/libraries), [Pathologic](https://www.drupal.org/project/pathologic), and [Display Suite](https://www.drupal.org/project/ds) modules and their dependencies. The Display Suite Extras and Display Suite UI modules that come with Display Suite are also required.

Then, install the necessary libraries. The libraries require [composer](https://getcomposer.org/) to install their dependencies, so make sure it's installed as well.

## MailChimp API Library for PHP

Download the [mailchimp-api-php](https://github.com/thinkshout/mailchimp-api-php) library and [extract it to one of the library directories](https://www.drupal.org/docs/7/modules/libraries-api/installing-an-external-library-that-is-required-by-a-contributed-module), e.g., `sites/all/libraries/mailchimp-api-php`. Then, install its dependencies:

    $ cd sites/all/libraries/mailchimp-api-php
    $ composer install --no-dev

The latest version tested and known to work is the v1.0.6 release.

## Minify

Download the [Minify](https://github.com/mrclay/minify) library and [extract it to one of the library directories](https://www.drupal.org/docs/7/modules/libraries-api/installing-an-external-library-that-is-required-by-a-contributed-module), e.g., `sites/all/libraries/minify`. Then, install its dependencies:

    $ cd sites/all/libraries/minify
    $ composer install --no-dev

The latest version tested and known to work is the v2.3.0 release.

## Emogrifier

Download the [Emogrifier](https://github.com/jjriv/emogrifier) library and [extract it to one of the library directories](https://www.drupal.org/docs/7/modules/libraries-api/installing-an-external-library-that-is-required-by-a-contributed-module), e.g., `sites/all/libraries/emogrifier`. Then, install its dependencies:

    $ cd sites/all/libraries/emogrifier
    $ composer install --no-dev

The latest version tested and known to work is the v1.1.0 release.

# Further Help and Documentation

The module includes further help and documentation, including plenty of screenshots, that can be accessed through Drupal's built-in help system. The page can be viewed by clicking the *Help* button in the Drupal toolbar and then clicking the *EmailMonkey* link.

# Credits

The 7.x-1.0 version was developed for and paid by [Pushing7](http://www.pushing7.com/).
