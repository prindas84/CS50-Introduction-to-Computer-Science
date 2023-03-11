# My Model Website - Content Management System (BASIC)
#### Video Demo:  https://www.facebook.com/groups/cs50/posts/2185243748289325/
#### Description:

My project is a simple website for a Modelling Agency to run their business with. It has a homepage, an about us page, a page to display the models who work for their agency, and a contact us page.

The project also works as the basic foundation to a Content Management System for the website. Although this particular project does not allow for the majority of functionality required with a Content Management System, it gives the basic building blocks to add to the back-end and develop a more complex system.

The back-end is accessed by the login page. The user details for this particular project are as follows:

Username: Admin
Password: 12345

###### Homepage, Header & Footer:

The homepage is a simple HTML page which contains the following:

Header - with custom CSS to underline the different menu options on hover, and also when the current page is being viewed.
Banner Image - spans the full-width of the page.
Textual Content - standard HTML text content using H-tags and P-tags.
Footer - which uses Flask blocks to show an Admin Login menu option to users who are not logged in, which changes to an Admin Dashboard link once the user has logged in.

###### About Us:

Floating Image - the image used on this page is floating to the right, which gives the text a wrap-around look.
Textual Content - the text uses various H-tags, P-tags, ordered lists, and unordered lists.

###### Models:

Dynamic Model Display - the models listed on this page, by name and image, are pulled from the back-end database of models. A logged in user can add or remove models from the database, and these changes will dynamically take effect on the front-end.

###### Contact Us:

Contact Form - the contact form displayed on this page will feed directly into the database. When a user submits the form, their enquiry details will be inserted into the database, and a logged in user can view the enquires that have been made through the back-end admin portal.
Error Messages - the contact form requires that the user submit their name, phone number, email address and a message. If these fields are not completed, an error message will display. The company name field is not required.
Automatic Page Scroll - in the case of an error message, the page is refreshed, and the user would normally be taken back to the top of the page. Because the error message is further down this page, they might not notice it, which creates a bad user experience. For this reason, on this page, when an error is triggered, the user will automatically be scrolled down to the error message once the page has refreshed, so they can understand the problem.
Success Message - once the user has submitted an enquiry successfully, a confirmation message will be displayed at the top of the page. Using CSS styling, this message will automatically disappear after a few seconds.

###### Admin Login:

This page will allow a user to login to the back-end of the website. The form will check that the user has entered a username and password, and that they are correct, before logging them in.

Once a user has successfully logged in, a success confirmation message will appear on the screen, and then disappear after a few seconds with the help of CSS styling.

###### Dashboard:

The dashboard provides the option for the user to view the enquiries that are in the system, view the models that are listed in the system, or logout of the system.

This page is only accessible by a logged in user.

###### Enquiries:

On the enquiry page, a logged in user can view all of the enquiries that have been submitted on the website. Once they are finished with the enquiry, they can delete it from the system.

The user can also return to the dashboard by click the Return To Dashboard button.

This page is only accessible by a logged in user.

###### Model Profiles:

On the model profiles page, a logged in user can view all of the models that have been submitted into the database. They also have the ability to remove a model from the database with the delete button. All models listed in the database will display on the front-end page.

The user can also return to the dashboard by clicking the Return To Dashboard button, or they can add a new model to the database by visiting the Add Model page.

This page is only accessible by a logged in user.

###### Add Model:

On the add model page, a logged in user can add a model to the database by submitting the details into the form. The form will check to make sure the user has submitted a name, phone number, email address, and uploaded an image of .JPG, .JPEG, or .PNG, and return an error message if these are not supplied.

The user can also return to the dashboard by click the Return To Dashboard button, or they can return to the previous page and view the Model Profiles by clicking the Return To .

This page is only accessible by a logged in user.
